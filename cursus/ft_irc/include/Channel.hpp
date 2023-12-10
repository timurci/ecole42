#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <set>

extern int	COMMENT; //The extern declaration in the header file informs the compiler that COMMENT is defined elsewhere (main.cpp).

struct Mode
{
    bool t;     //(Topic):          Only channel operators can change the channel's topic.
    bool m;     //(Moderated):      Only channel operators and voiced users can send messages.
    bool l;     //(User Limit):     Limits the number of users who can join the channel.
    bool k;     //(Key):            A password is required to join the channel.
    bool s;     //(Secret):         The channel does not appear in the channel list, and users must know the name to join.
    bool p;     //(Private):        Hides the channel from being listed in the server's public list.
    bool v;     //(Voice):          Users with this mode can speak in a moderated channel, even if it's set to +m.
    bool o;     //(Operator):       Channel operators have elevated privileges.
    bool b;     //(Ban):            Users can be banned from the channel by their nickname or hostname.

    Mode()
        : t(false), m(false), l(false), k(false), s(false), p(false), v(false), o(false), b(false)
    {}
};

class Channel
{
	private:
		std::string						_name;
		std::map<const int, Client*> 	_clients;
		std::map<const int, Client*> 	_ableClients;
		std::map<const int, Client*> 	_bannedClients;
		std::set<std::string>			_kickedClients;
		std::map<const int, Client*> 	_voicedClients;
		std::string						_topic;
		std::string						_password;
		std::map<const int, Client*> 	_operators;
		std::string						_operatorPassword;
		std::string						_messageLog;	
		unsigned						_capacity;
		struct Mode						_mode;

		int						addAbleClient(Client&);
		int						kickAbleClient(Client&);
		int						dismissBannedClient(Client&);

	public:
		int						addToKickedClients(Client&);	
		int 					addToKickedClients(const std::string &client_nick);

	public:
		Channel(const std::string &theChannelName);
		~Channel();

		std::string&			getName();
		void					setName(std::string);

		std::map<const int, Client*>&	getClients();
		std::map<const int, Client*>&	getBannedClients();
		std::map<const int, Client*>&	getAbleClients();
		std::set<std::string>&			getKickedClients();
		std::map<const int, Client*>&	getVoicedClients();

		const Client*					getClientByNick(const std::string &) const;

		std::string&			getTopic();
		void					setTopic(const std::string&);
		std::string&			getPassword();
		void					setPassword(const std::string&);
		void					removePassword();
		
		std::map<const int, Client*>&	getOperators();

		std::string&			getMessageLog() const;
		int						getCapacity() const;
		void					setCapacity(int);
		Mode&					getMode();
		std::string				getStringMode(void) const;

		void					broadcastToChannel(const std::string &message);
		int						addClient(Client&);

		int						addOperator(Client&);
		int						dismissOperator(Client&);
		bool					isOperator(Client&);

		void					toggleMode(const char, bool);		

		int						removeClientFromChannel(Client&);
		int						kickClient(Client&);
		int						banClient(Client&);
		int						restoreBannedClient(Client&);
		bool					isBanned(Client&) const;
		int						addToVoiced(Client&);
		int						dismissVoicedClient(Client&);
		bool					isVoiced(Client&) const;
		bool					doesClientExist(const Client &client) const;
		bool					doesClientExist(const std::string &client_nick) const;
		void    				PrintChannel();
};


void printChannel(const Channel& ch);

#endif
