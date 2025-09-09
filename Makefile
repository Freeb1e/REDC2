.PHONY: build run RUN clean

build:
	verilator --trace -cc REDC3329.sv --exe tb_REDC3329.cpp --top-module REDC3329_pipeline -Mdir obj_dir
	$(MAKE) -C obj_dir -f VREDC3329_pipeline.mk VREDC3329_pipeline

run: build
	./obj_dir/VREDC3329_pipeline

RUN: run

clean:
	rm -rf obj_dir waveform.vcd