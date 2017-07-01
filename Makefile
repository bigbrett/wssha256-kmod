all:
	-rm -rf build
	-mkdir build
	@cd build ;\
	cmake -DKERNEL_SRC="foo" .. ;\
	make ;


clean:
	-rm -rfv build/ 
	-rm -fv $(shell find . | grep ~$$)
