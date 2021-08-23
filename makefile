all: barectf

barectf:
	@mkdir -p source
	cd config && barectf generate --code-dir ../source/ --headers-dir ../include/ config.yaml
	mv barectf/metadata barectf/trace/metadata
.PHONY: barectf