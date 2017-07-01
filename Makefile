all:
	-rm -rf build
	-mkdir build
	@cd build ;\
	cmake .. ;\
	make ;


clean:
	-rm -rfv build/ 
	-rm -fv $(shell find . | grep ~$$)
