struct musb_hw_ep {
	int a;
	struct musb     *musb;
	struct s1 sb;
	struct s2 sc;
};