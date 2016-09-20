asteroides:
	gcc servidor.c -lX11 -L/user/X11R6/lib -lm -o servidor
	gcc cliente.c -lX11 -L/user/X11R6/lib -lm -o cliente
