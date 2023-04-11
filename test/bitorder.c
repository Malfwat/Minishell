int	main(void)
{
	char	b = 2;
	int i = 8;

	while (i--)
	{
		printf("%i", b >> i & 1);
	}
}
