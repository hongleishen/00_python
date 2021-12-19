/* i2c driver borrow from linux */

#define pr_fmt(fmt) "i2c-core: " fmt

//#include "i2c-core.h"
#include "i2c.h"
#include "i2c_priv.h"
#define in_atomic()		0
#define irqs_disabled()		0


#define I2C_ADDR_OFFSET_TEN_BIT	0xa000
#define I2C_ADDR_OFFSET_SLAVE	0x1000

#define I2C_ADDR_7BITS_MAX	0x77
#define I2C_ADDR_7BITS_COUNT	(I2C_ADDR_7BITS_MAX + 1)

/*
 * core_lock protects i2c_adapter_list, and guarantees that device detection,
 * deletion of detected devices, and attach_adapter calls are serialized
 */
static DEFINE_MUTEX(core_lock);
static LIST_HEAD(i2c_adapter_list);

/* Return a unique address which takes the flags of the client into account */
static unsigned short i2c_encode_flags_to_addr(struct i2c_client *client)
{
	unsigned short addr = client->addr;

	/* For some client flags, add an arbitrary offset to avoid collisions */
	if (client->flags & I2C_CLIENT_TEN)
		addr |= I2C_ADDR_OFFSET_TEN_BIT;

	if (client->flags & I2C_CLIENT_SLAVE)
		addr |= I2C_ADDR_OFFSET_SLAVE;

	return addr;
}

/* This is a permissive address validity check, I2C address map constraints
 * are purposely not enforced, except for the general call address. */
static inline int i2c_check_addr_validity(unsigned addr, unsigned short flags)
{
	if (flags & I2C_CLIENT_TEN) {
		/* 10-bit address, all values are valid */
		if (addr > 0x3ff)
			return -EINVAL;
	} else {
		/* 7-bit address, reject the general call address */
		if (addr == 0x00 || addr > 0x7f)
			return -EINVAL;
	}
	return 0;
}

/* And this is a strict address validity check, used when probing. If a
 * device uses a reserved address, then it shouldn't be probed. 7-bit
 * addressing is assumed, 10-bit address devices are rare and should be
 * explicitly enumerated. */
int i2c_check_7bit_addr_validity_strict(unsigned short addr)
{
	/*
	 * Reserved addresses per I2C specification:
	 *  0x00       General call address / START byte
	 *  0x01       CBUS address
	 *  0x02       Reserved for different bus format
	 *  0x03       Reserved for future purposes
	 *  0x04-0x07  Hs-mode master code
	 *  0x78-0x7b  10-bit slave addressing
	 *  0x7c-0x7f  Reserved for future purposes
	 */
	if (addr < 0x08 || addr > 0x77)
		return -EINVAL;
	return 0;
}

static int i2c_check_addr_busy(struct i2c_adapter *adapter, int addr)
{
	int result = 0;
	struct i2c_client *c;

	if (!list_empty(&adapter->client_list)) {
		list_for_each_entry(c, &adapter->client_list, to_adap) {
			if (i2c_encode_flags_to_addr(c) == addr) {
				result = 1;
				break;
			}
		}
	}
	return result;
}

/**
 * i2c_adapter_lock_bus - Get exclusive access to an I2C bus segment
 * @adapter: Target I2C bus segment
 * @flags: I2C_LOCK_ROOT_ADAPTER locks the root i2c adapter, I2C_LOCK_SEGMENT
 *	locks only this branch in the adapter tree
 */
static void i2c_adapter_lock_bus(struct i2c_adapter *adapter,
				 unsigned int flags)
{
//	rt_mutex_lock(&adapter->bus_lock);
	FRTOS_mutex_lock(adapter->bus_lock);
}

/**
 * i2c_adapter_trylock_bus - Try to get exclusive access to an I2C bus segment
 * @adapter: Target I2C bus segment
 * @flags: I2C_LOCK_ROOT_ADAPTER trylocks the root i2c adapter, I2C_LOCK_SEGMENT
 *	trylocks only this branch in the adapter tree
 */
static int i2c_adapter_trylock_bus(struct i2c_adapter *adapter,
				   unsigned int flags)
{
	return rt_mutex_trylock(&adapter->bus_lock);
}

/**
 * i2c_adapter_unlock_bus - Release exclusive access to an I2C bus segment
 * @adapter: Target I2C bus segment
 * @flags: I2C_LOCK_ROOT_ADAPTER unlocks the root i2c adapter, I2C_LOCK_SEGMENT
 *	unlocks only this branch in the adapter tree
 */
static void i2c_adapter_unlock_bus(struct i2c_adapter *adapter,
				   unsigned int flags)
{
//	rt_mutex_unlock(&adapter->bus_lock);
	FRTOS_mutex_unlock(adapter->bus_lock);
}

/**
 * i2c_new_device - instantiate an i2c device
 * @adap: the adapter managing the device
 * @info: describes one I2C device; bus_num is ignored
 * Context: can sleep
 *
 * Create an i2c device. Binding is handled through driver model
 * probe()/remove() methods.  A driver may be bound to this device when we
 * return from this function, or any later moment (e.g. maybe hotplugging will
 * load the driver module).  This call is not appropriate for use by mainboard
 * initialization logic, which usually runs during an arch_initcall() long
 * before any i2c_adapter could exist.
 *
 * This returns the new i2c client, which may be saved for later use with
 * i2c_unregister_device(); or NULL to indicate an error.
 */
struct i2c_client *
i2c_new_device(struct i2c_adapter *adap, struct i2c_board_info const *info)
{
	struct i2c_client	*client;
	int			status;

	client = pvPortMalloc(sizeof *client);
	if (!client)
		return NULL;

	client->adapter = adap;

	client->flags = info->flags;
	client->addr = info->addr;
	INIT_LIST_HEAD(&client->to_adap);

	client->irq = info->irq;
	strncpy(client->name, info->type, sizeof(client->name));

	status = i2c_check_addr_validity(client->addr, client->flags);
	if (status) {
		dev_err(&adap->dev, "Invalid %d-bit I2C address 0x%02hx\n",
			client->flags & I2C_CLIENT_TEN ? 10 : 7, client->addr);
		goto out_err_silent;
	}

	/* Check for address business */
	status = i2c_check_addr_busy(adap, i2c_encode_flags_to_addr(client));
	if (status)
		goto out_err;

	mutex_lock(&core_lock);
	list_add_tail(&client->to_adap, &adap->client_list);
	mutex_unlock(&core_lock);
	client->adapter = adap;
	return client;

out_err:
	dev_err(&adap->dev,
		"Failed to register i2c client %s at 0x%02x (%d)\n",
		client->name, client->addr, status);
out_err_silent:
	vPortFree(client);
	return NULL;
}
EXPORT_SYMBOL(i2c_new_device);


/**
 * i2c_unregister_device - reverse effect of i2c_new_device()
 * @client: value returned from i2c_new_device()
 * Context: can sleep
 */
void i2c_unregister_device(struct i2c_client *client)
{
	mutex_lock(&core_lock);
	list_del_init(&client->to_adap);
	client->adapter = 0;
	mutex_unlock(&core_lock);
}
EXPORT_SYMBOL(i2c_unregister_device);
#if x
//suntongce warning  completion give
static void i2c_adapter_dev_release(struct device *dev)
{
	struct i2c_adapter *adap = to_i2c_adapter(dev);
	FRTOS_complete(adap->dev_released);
}
#endif

static const struct i2c_lock_operations i2c_adapter_lock_ops = {
	.lock_bus =    i2c_adapter_lock_bus,
	.trylock_bus = i2c_adapter_trylock_bus,
	.unlock_bus =  i2c_adapter_unlock_bus,
};

static int i2c_register_adapter(struct i2c_adapter *adap)
{
	int res = -EINVAL;
#if 0
	/* Sanity checks */
	if (WARN(!adap->name[0], "i2c adapter has no name"))
		goto out_list;
#endif
	if (!adap->algo) {
		pr_err("adapter '%s': no algo supplied!\n", adap->name);
		return res;
	}

	if (!adap->lock_ops)
		adap->lock_ops = &i2c_adapter_lock_ops;

//	rt_mutex_init(&adap->bus_lock);
//	rt_mutex_init(&adap->mux_lock);
	FRTOS_mutex_create(&adap->bus_lock);
//	mutex_init(&adap->userspace_clients_lock);
//	INIT_LIST_HEAD(&adap->userspace_clients);

	/* Set default timeout to 1 second if not already set */
	if (adap->timeout == 0)
		adap->timeout = configTICK_RATE_HZ;
	return 0;
}

/**
 * i2c_add_adapter - declare i2c adapter, use dynamic bus number
 * @adapter: the adapter to add
 * Context: can sleep
 *
 * This routine is used to declare an I2C adapter when its bus number
 * doesn't matter or when its bus number is specified by an dt alias.
 * Examples of bases when the bus number doesn't matter: I2C adapters
 * dynamically added by USB links or PCI plugin cards.
 *
 * When this returns zero, a new bus number was allocated and stored
 * in adap->nr, and the specified adapter became available for clients.
 * Otherwise, a negative errno value is returned.
 */
int i2c_add_adapter(struct i2c_adapter *adapter, int id)
{
	int ret = -EINVAL;	

	if (id >= 0) {
		adapter->nr = id;
		ret = i2c_register_adapter(adapter);
		if (!ret) {
			mutex_lock(&core_lock);
			list_add_tail(&adapter->adap_list, &i2c_adapter_list);
			mutex_unlock(&core_lock);
		}
	}
	return ret;
}
EXPORT_SYMBOL(i2c_add_adapter);


int i2c_add_numbered_adapter(struct i2c_adapter *adap)
{
	if (adap->nr == -1) /* -1 means dynamically assign bus id */
		return -EINVAL;
	return i2c_add_adapter(adap, adap->nr);
}
EXPORT_SYMBOL(i2c_add_numbered_adapter);

void i2c_del_adapter(struct i2c_adapter *adap)
{
	struct i2c_adapter *found;
	struct i2c_client *client;
	int find = 0;

	/* First make sure that this adapter was ever added */
	mutex_lock(&core_lock);
	list_for_each_entry(found, &i2c_adapter_list, adap_list) {
		if (found->nr == adap->nr) {
			find = 1;
			break;
		}
	}
	mutex_unlock(&core_lock);
	if (!find) {
		pr_debug("attempting to delete unregistered adapter [%s]\n", adap->name);
		return;
	}

	mutex_lock(&core_lock);
	list_for_each_entry(client, &adap->client_list, to_adap){
		list_del_init(&client->to_adap);
		client->adapter = 0;
		vPortFree(client);
	}
	mutex_unlock(&core_lock);

	/* wait until all references to the device are gone
	 *
	 * FIXME: This is old code and should ideally be replaced by an
	 * alternative which results in decoupling the lifetime of the struct
	 * device from the i2c_adapter, like spi or netdev do. Any solution
	 * should be thoroughly tested with DEBUG_KOBJECT_RELEASE enabled!
	 */
	adap->dev_released = FRTOS_completion_init();
	FRTOS_completion_wait(adap->dev_released, 0xFFFFFFFF);

	/* free bus id */
	mutex_lock(&core_lock);
	list_del_init(&adap->adap_list);
	mutex_unlock(&core_lock);
	vPortFree(adap);
	adap = 0;
}
EXPORT_SYMBOL(i2c_del_adapter);

/* ----------------------------------------------------
 * the functional interface to the i2c busses.
 * ----------------------------------------------------
 */

/**
 * __i2c_transfer - unlocked flavor of i2c_transfer
 * @adap: Handle to I2C bus
 * @msgs: One or more messages to execute before STOP is issued to
 *	terminate the operation; each message begins with a START.
 * @num: Number of messages to be executed.
 *
 * Returns negative errno, else the number of messages executed.
 *
 * Adapter lock must be held when calling this function. No debug logging
 * takes place. adap->algo->master_xfer existence isn't checked.
 */
static int __i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)
{
	TickType_t orig_jiffies;
	int ret, try;

	/* Retry automatically on arbitration loss */
	orig_jiffies = xTaskGetTickCount();
	for (ret = 0, try = 0; try <= adap->retries; try++) {
		ret = adap->algo->master_xfer(adap, msgs, num);
		if (ret != -EAGAIN)
			break;
		if (xTaskGetTickCount() > orig_jiffies + adap->timeout)
			break;
	}

	return ret;
}

/**
 * i2c_transfer - execute a single or combined I2C message
 * @adap: Handle to I2C bus
 * @msgs: One or more messages to execute before STOP is issued to
 *	terminate the operation; each message begins with a START.
 * @num: Number of messages to be executed.
 *
 * Returns negative errno, else the number of messages executed.
 *
 * Note that there is no requirement that each message be sent to
 * the same slave address, although that is the most common model.
 */
int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)
{
	int ret;

	/* REVISIT the fault reporting model here is weak:
	 *
	 *  - When we get an error after receiving N bytes from a slave,
	 *    there is no way to report "N".
	 *
	 *  - When we get a NAK after transmitting N bytes to a slave,
	 *    there is no way to report "N" ... or to let the master
	 *    continue executing the rest of this combined message, if
	 *    that's the appropriate response.
	 *
	 *  - When for example "num" is two and we successfully complete
	 *    the first message but get an error part way through the
	 *    second, it's unclear whether that should be reported as
	 *    one (discarding status on the second message) or errno
	 *    (discarding status on the first one).
	 */

	if (adap->algo->master_xfer) {
#if 0//def DEBUG
		for (ret = 0; ret < num; ret++) {
			dev_dbg(&adap->dev,
				"master_xfer[%d] %c, addr=0x%02x, len=%d%s\n",
				ret, (msgs[ret].flags & I2C_M_RD) ? 'R' : 'W',
				msgs[ret].addr, msgs[ret].len,
				(msgs[ret].flags & I2C_M_RECV_LEN) ? "+" : "");
		}
#endif
//suntongce warning trylock implement
		if (in_atomic() || irqs_disabled()) {
			ret = i2c_trylock_bus(adap, I2C_LOCK_SEGMENT);
			if (!ret)
				/* I2C activity is ongoing. */
				return -EAGAIN;
		} else {
			i2c_lock_bus(adap, I2C_LOCK_SEGMENT);
		}

		ret = __i2c_transfer(adap, msgs, num);
		i2c_unlock_bus(adap, I2C_LOCK_SEGMENT);

		return ret;
	} else {
		dev_dbg(&adap->dev, "I2C level transfers not supported\n");
		return -EOPNOTSUPP;
	}
}
EXPORT_SYMBOL(i2c_transfer);

/**
 * i2c_master_send - issue a single I2C message in master transmit mode
 * @client: Handle to slave device
 * @buf: Data that will be written to the slave
 * @count: How many bytes to write, must be less than 64k since msg.len is u16
 *
 * Returns negative errno, or else the number of bytes written.
 */
int i2c_master_send(const struct i2c_client *client, const char *buf, int count)
{
	int ret;
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg;

	msg.addr = client->addr;
	msg.flags = client->flags & I2C_M_TEN;
	msg.len = count;
	msg.buf = (unsigned char *)buf;

	ret = i2c_transfer(adap, &msg, 1);

	/*
	 * If everything went ok (i.e. 1 msg transmitted), return #bytes
	 * transmitted, else error code.
	 */
	return (ret == 1) ? count : ret;
}
EXPORT_SYMBOL(i2c_master_send);

/**
 * i2c_master_recv - issue a single I2C message in master receive mode
 * @client: Handle to slave device
 * @buf: Where to store data read from slave
 * @count: How many bytes to read, must be less than 64k since msg.len is u16
 *
 * Returns negative errno, or else the number of bytes read.
 */
int i2c_master_recv(const struct i2c_client *client, char *buf, int count)
{
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg;
	int ret;

	msg.addr = client->addr;
	msg.flags = client->flags & I2C_M_TEN;
	msg.flags |= I2C_M_RD;
	msg.len = count;
	msg.buf =(unsigned char *) buf;

	ret = i2c_transfer(adap, &msg, 1);

	/*
	 * If everything went ok (i.e. 1 msg received), return #bytes received,
	 * else error code.
	 */
	return (ret == 1) ? count : ret;
}
EXPORT_SYMBOL(i2c_master_recv);

struct i2c_adapter *i2c_get_adapter(int nr)
{
	struct i2c_adapter *adapter = NULL;
	int find = 0;

	mutex_lock(&core_lock);
	list_for_each_entry(adapter, &i2c_adapter_list, adap_list) {
		if (adapter->nr == nr) {
			find = 1;
			break;
		}
	}
	if (!find) {
		adapter = NULL;
	}
	mutex_unlock(&core_lock);
	return adapter;
}
EXPORT_SYMBOL(i2c_get_adapter);


#if 1//defined(CONFIG_I2C_SLAVE)
int i2c_slave_register(struct i2c_client *client, i2c_slave_cb_t slave_cb)
{
    int ret;

    if (!client || !slave_cb) {
        dev_err(&client->dev, "insufficient data\n");
        return -EINVAL;
    }

    if (!(client->flags & I2C_CLIENT_SLAVE))
        dev_warn(&client->dev, "%s: client slave flag not set. You might see address collisions\n",
             __func__);

    if (!(client->flags & I2C_CLIENT_TEN)) {
        /* Enforce stricter address checking */
        ret = i2c_check_7bit_addr_validity_strict(client->addr);
        if (ret) {
            dev_err(&client->dev, "%s: invalid address\n", __func__);
            return ret;
        }
    }

    if (!client->adapter->algo->reg_slave) {
        dev_err(&client->dev, "%s: not supported by adapter\n", __func__);
        return -EOPNOTSUPP;
    }

    client->slave_cb = slave_cb;

    i2c_lock_adapter(client->adapter);
    ret = client->adapter->algo->reg_slave(client);
    i2c_unlock_adapter(client->adapter);

    if (ret) {
        client->slave_cb = NULL;
        dev_err(&client->dev, "%s: adapter returned error %d\n", __func__, ret);
    }

    return ret;
}
EXPORT_SYMBOL(i2c_slave_register);

int i2c_slave_unregister(struct i2c_client *client)
{
    int ret;

    if (!client->adapter->algo->unreg_slave) {
        dev_err(&client->dev, "%s: not supported by adapter\n", __func__);
        return -EOPNOTSUPP;
    }

    i2c_lock_adapter(client->adapter);
    ret = client->adapter->algo->unreg_slave(client);
    i2c_unlock_adapter(client->adapter);

    if (ret == 0)
        client->slave_cb = NULL;
    else
        dev_err(&client->dev, "%s: adapter returned error %d\n", __func__, ret);

    return ret;
}
EXPORT_SYMBOL_GPL(i2c_slave_unregister);
#endif

void i2c_probe(void)
{
	mutex_init(&core_lock);
}
SUBSYS_INIT(i2c_probe);
