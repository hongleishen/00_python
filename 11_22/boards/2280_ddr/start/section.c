char __bss_start[0] __attribute__((section(".__bss_start")));
char __bss_end[0] __attribute__((section(".__bss_end")));

void hang(void)
{
	while(1) {};
}
void __div0 (void)
{
	hang();
}
