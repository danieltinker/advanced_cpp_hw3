.PHONY: all clean

all:
	$(MAKE) -C Simulator
	$(MAKE) -C Algorithm
	$(MAKE) -C GameManager

clean:
	$(MAKE) -C Simulator clean
	$(MAKE) -C Algorithm clean
	$(MAKE) -C GameManager clean
