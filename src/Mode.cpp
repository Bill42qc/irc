#include "Mode.hpp"

Mode::~Mode(){
}
 
void Mode::exe() const
{
	channel_.broadcastEveryone("set up un mode style test\n");
	channel_.setPassword("PASSWORD");
	channel_.setNeedPassword_(!channel_.getNeedPassword());
}