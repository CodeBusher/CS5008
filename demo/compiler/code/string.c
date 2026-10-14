typedef struct String
{
	char *data;
	long count;
} String;

#define PRINT_STRING(s) (int)(s).count, (s).data
#define str_lit(s) (String){(char *)(s), sizeof(s) - 1}

bool strings_match(String s1, String s2)
{
	if (s1.count != s2.count)
	{
		return false;
	}
	
	for (long i = 0; i < s1.count; ++i)
	{
		if (s1.data[i] != s2.data[i])
		{
			return false;
		}
	}
	
	return true;
}
