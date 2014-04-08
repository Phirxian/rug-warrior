* makes the sbot beep when put in a loop.
	LDAA	PORTA
	EORA	#$8
	STAA	PORTA

