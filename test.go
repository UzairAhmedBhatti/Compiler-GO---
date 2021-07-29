Integer: i, j, count, temp;
i := 1;
j := 0;
count := 0;
print(count);
count := 0;

if j = 0:{
	while i <= 10:{
		j := 0;
		while j <= 10:{
			count := count + 1;
			j := j + 1;
		}
		i := i + 1;
	}
}

print(count);

if count < 100:
{
	print(count);
}
elif count < 500:
{
	if count > 100:
	{
		print(j);
		print(count);
		i := ((100 / 2) * (20 / 10)) * (10 * count) ;
		print(i);
		
	}
}

