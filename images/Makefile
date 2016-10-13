all:
	find . -iname '*.jpg' -execdir convert {} -depth 8 -set filename:namenwidth "%[t]_%[w]" '%[filename:namenwidth].rgb' \;
	find . -iname '*.png' -execdir convert {} -depth 8 -set filename:namenwidth "%[t]_%[w]" '%[filename:namenwidth].rgb' \;
	find . -iname '*.rgb' -exec ../imageconvert/imgconvert {} \;

clean:
	find . -iname '*.rgb' -exec rm -f {} \;
	find . -iname '*.img' -exec rm -f {} \;

arm-none-eabi: all
