{
	"targets": [
		{
			"target_name": "addon",
			"sources": [
				"src/addon.cc",
				"src/myobject.cc"
			],
			"include_dirs": [
				"include"
			],
			"dependencies": [
				"deps/scarab/libscarab.gyp:scarab"
			]
		}
	]
}