#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static void	broadcastToAllChannelMembers2(Channel &channel, std::string announcement)
{
	std::map<const int, Client*>::iterator member = channel.getClients().begin();
	
	while (member != channel.getClients().end())
	{
		member->second->appendSendBuffer(announcement);
		member++;
	}	
}


static bool	valid(const std::string &str)
{
	if (str.empty())
		return (0);

	for (size_t i = 0; i < str.length(); i++) 
	{
        if (!isalpha(str[i])) 
            return (false);
    }
    return (true);
}


void 	keyChannelMode(Client &operat, const std::string &password, Channel &channel, const std::string &mode)
{
	if (!channel.isOperator(operat))
	{
		operat.appendSendBuffer(ERR_NOTANOPERATOROFCHANNEL(operat.getNickname(), channel.getName()));
		return ;
	}
	if (!valid(password))
	{
		operat.appendSendBuffer(ERR_INVALIDKEY(password));
		return ;
	}
	if (mode[0] == '+')
	{
		channel.setPassword(password);//This function also toggles the 'k' mode to true.
		broadcastToAllChannelMembers2(channel, MODE_CHANNELMSGWITHPARAM(channel.getName(), "+k", password));
	}
	else if (mode[0] == '-')
	{
		channel.removePassword();	//This function also toggles the 'k' mode to false.
		broadcastToAllChannelMembers2(channel, MODE_CHANNELMSGWITHPARAM(channel.getName(), "-k", password));
	}	
}