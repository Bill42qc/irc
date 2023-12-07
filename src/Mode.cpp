#include "Mode.hpp"

Mode::~Mode(){
}
 
void Mode::exe() const
{
	channel_.broadcastEveryone("set up un mode style test\n");
}