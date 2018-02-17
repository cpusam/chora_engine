#ifndef CHORA_JSON
#define CHORA_JSON

class JSONconvert
{
	public:
		JSONconvert ( Entity * entity );
		std::string to_json ();
		int getId();
};

#endif
