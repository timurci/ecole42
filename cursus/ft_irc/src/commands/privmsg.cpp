#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

/**
 * @brief PRIVMSG is used to send private messages between users, as well as to
   send messages to channels.  <msgtarget> is usually the nickname of
   the recipient of the message, or a channel name.
   
   Parameters: <msgtarget> <text to be sent>

   The <msgtarget> parameter may also be a host mask (#<mask>) or server
   mask ($<mask>).  In both cases the server will only send the PRIVMSG
   to those who have a server or host matching the mask.  The mask MUST
   have at least 1 (one) "." in it and no wildcards following the last
   ".".  This requirement exists to prevent people sending messages to
   "#*" or "$*", which would broadcast to all users.  Wildcards are the
   '*' and '?'  characters.  This extension to the PRIVMSG command is
   only available to operators.

   Numeric Replies:
    ERR_NOSUCHNICK (401) -OK
    ERR_CANNOTSENDTOCHAN (404)
    ERR_TOOMANYTARGETS (407) 
    ERR_NORECIPIENT (411) -OK
    ERR_NOTEXTTOSEND (412) -OK
    ERR_NOTOPLEVEL (413)
    ERR_WILDTOPLEVEL (414)
    RPL_AWAY (301)

   @param server
   @param client_fd User sending a msg
   @param cmd_infos Structure w/ prefix, command name and message

   EXAMPLES :
   Examples:

   :Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message ?
    ; Message from Angel to Wiz.

   PRIVMSG Angel :yes I'm receiving it !
	; Command to send a message to Angel.

   PRIVMSG jto@tolsun.oulu.fi :Hello !
	; Command to send a message to a user on server tolsun.oulu.fi with
    username of "jto".

   PRIVMSG kalt%millennium.stealth.net@irc.stealth.net :Are you a frog?
   	; Message to a user on server irc.stealth.net with username of "kalt", 
	and connected from the host millennium.stealth.net.

   PRIVMSG kalt%millennium.stealth.net :Do you like cheese?
    ; Message to a user on the local server with username of "kalt", and
    connected from the host millennium.stealth.net.

   PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
   	; Message to the user with nickname Wiz who is connected from the host
    tolsun.oulu.fi and has the username "jto".

   PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
    ; Message to everyone on a server which has a name matching *.fi.

   PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
    ; Message to all users who come from a host which has a name matching *.edu.

	useful link : https://irssi.org/documentation/help/msg/
   https://modern.ircdocs.horse/#errnosuchnick-401
   http://abcdrfc.free.fr/rfc-vf/rfc1459.html (errors)
   https://askubuntu.com/questions/855881/irssi-where-do-private-messages-go (how to use IRSSI)
   https://datatracker.ietf.org/doc/html/rfc2812#section-3.3 RFC DE REFERENCE
 * 
 */

static void  broadcastToChannel(Client &client, Channel &channel,
								const std::string &target, const std::string &msg_to_send)
{
	std::set<std::string> &banned_users = channel.getBannedClients();
	std::set<std::string>::const_iterator it;

	for (it = banned_users.begin(); it != banned_users.end(); it++)
	{
	if (*it == client.getNickname())
		{
			std::cout << "[Server] " << client.getNickname() << " is banned from the channel and can't send messages anymore" << std::endl;
			return ;
		}
	}

	if (!channel.doesClientExist(client))
	{
		client.appendSendBuffer(ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
		return ;
	}

	//std::set<std::string>::const_iterator it_kicked = channel.getKickedClients().find(client.getNickname());
	
	//if (it_kicked != channel.getKickedClients().end())
	//{
	//	std::cout << "[" << YELLOW << "Server" << RESET << "] " << RED
	//		<< client.getNickname() << RESET << " is kicked from the channel and can't send messages anymore" << std::endl;
	//	return ;
	//}
   
	if (channel.getMode().m)
   {
      if (channel.isOperator(client) == false /*&& channel.isVoiced(client) == false */)
      {
         client.appendSendBuffer(ERR_CANNOTSENDTOCHAN(client.getNickname(), channel.getName()));
         return ;
      }
   }
   // Envoi le message aux users du channel 
   std::map<const int, Client*>::iterator member = channel.getClients().begin(); // debut de la liste des clients du channel
   while (member != channel.getClients().end())
   {
		// PRIVMSG_DEBUG
		//std::cout << "=============== BROADCASTING!!!!!! ===============" << std::endl;
		//stdstd::cout << "Client : " << client.getNickname() << "\ttarget : " << target << std::endl;
		//stdstd::cout << "Member : " << member->second->getNickname() << std::endl;
		//std::cout << "msg_to_send : " << msg_to_send << std::endl;
		//std::cout << "SENT : " << RPL_PRIVMSG(client.getNickname(), client.getUsername(), target, msg_to_send);
    	if (member->second->getClientFd() != client.getClientFd()) // preventing from sending the message to the sender
		{
        	member->second->appendSendBuffer(RPL_PRIVMSG(client.getNickname(), client.getUsername(), target, msg_to_send));
			std::cout << "Message sent!" << std::endl;
		}
    	member++;
   }
}

static bool isUserinChannel(Client &client, Channel &channel)
{
   if (channel.getClients().find(client.getClientFd()) != channel.getClients().end())
      return (true);
   return (false);
}

void	privmsg(Client &client, const t_cmd_info &cmd_info,
			std::map<const int, Client> &client_list,
			std::map<std::string, Channel> &channel_list)
{ 
   std::string target;
   std::string msg_to_send;
  
   // Parsing message 
   size_t      delimiter = cmd_info.params.rfind(":"); // consider using find()
   if (delimiter == std::string::npos) // pas de : -> donc pas de message
   {
      client.appendSendBuffer(ERR_NOTEXTTOSEND(client.getNickname()));
      return ;
   }
   target = cmd_info.params.substr(0, delimiter - 1); // s'arrete avant le delimiter
   if (target.empty())
   {
      client.appendSendBuffer(ERR_NORECIPIENT(client.getNickname()));
      return ;
   }
      
   msg_to_send = cmd_info.params.substr(delimiter);

   delimiter = target.find(" ");
   if (delimiter != std::string::npos) // permet de gérer le double :: et les espaces de la fin
      target.erase(delimiter);  
   
   // Channel case
   if (target[0] == '#')
   {
      std::map<std::string, Channel>::iterator it_channel = channel_list.find(target.substr(1)); // find channel name by skipping the '#' character

      if (it_channel == channel_list.end())
         client.appendSendBuffer(ERR_NOSUCHNICK(client.getNickname(), target));
      else
         broadcastToChannel(client, it_channel->second, target, msg_to_send);
   }
   // user case
   else
   {
      std::map<std::string, Channel>::iterator it_channel = channel_list.find(target); // find channel name
     
      std::map<const int, Client>::iterator it_target = client_list.begin();
      while (it_target!=client_list.end())
      {
         if (it_target->second.getNickname() == target)
             break;
         it_target++;
      }
      if (it_target == client_list.end() && it_channel == channel_list.end()) // user and channel doesn't exist
      {
         //if (target == "chatGTP")
         //   bot(server, client_fd, it_client, msg_to_send);
         //else
            client.appendSendBuffer(ERR_NOSUCHNICK(client.getNickname(), target));   
      }
      else
      {
         if (it_target == client_list.end())
         {
            if (isUserinChannel(client, it_channel->second) == true)
            {
               target.insert(1, "#");
               broadcastToChannel(client, it_channel->second, target, msg_to_send);
            }
            else
               client.appendSendBuffer(ERR_NOSUCHNICK(client.getNickname(), target));
         }
         else
            it_target->second.appendSendBuffer(RPL_PRIVMSG(client.getNickname(), client.getUsername(), target, msg_to_send));
      }
   }
}
