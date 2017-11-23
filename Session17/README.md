This assignment was to implement a compact eprintf() function that supports the following conversion specifications:
%c
%s
%d
%x and %X
%%
Between % and the conversion character, there may be:
	A - sign that signifies a left alignment of argument
	A decimal number that specifies the min field width
	The default pad char is ' ', if the decimal number begins with a 0, padd with 0
