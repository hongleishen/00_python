abc = 688
t_sum = 0
def tang_add(tang_list):
    global t_sum
    for i in range(len(tang_list)):
        t_sum += tang_list[i]
    return t_sum

tang_list= [1,2,3,4,5]
print(tang_add(tang_list))