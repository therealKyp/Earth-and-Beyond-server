using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Collections;

using Meebey.SmartIrc4net;

namespace ToolsLauncher
{
    public partial class IRCMessenger : Form
    {
    #region Varables
        string m_IRCChannel = "#test";
        string m_IRCServer = "eservices.dyndns.org";
        string m_Nick;
        int m_IRCPort = 6667;

        IrcClient m_IRCClient;
        Thread m_IrcThread;

        TreeNode m_UserTree = new TreeNode();

        // List of private message windows
        List<PrivateMessage> m_PrivateWindows = new List<PrivateMessage>();

        // List of online users
        List<String> m_OnlineUsers = new List<string>();
    #endregion

    #region     Delegates
        public delegate void ActionDelegateNoParams();
        public delegate void ActionDelegateIrcEvent(IrcEventArgs e);
    #endregion

    #region IRC Connector
        public IRCMessenger()
        {
            InitializeComponent();
            m_Nick = SQLData.User;
        }

        public void ListenThread()
        {
            m_IRCClient.Listen();
        }
    #endregion

    #region Form Messages
        private void IRCMessenger_Load(object sender, EventArgs e)
        {
            m_IRCClient = new IrcClient();

            // UTF-8 test
            m_IRCClient.Encoding = System.Text.Encoding.UTF8;
            // wait time between messages, we can set this lower on own irc servers
            m_IRCClient.SendDelay = 200;
            // we use channel sync, means we can use irc.GetChannel() and so on
            m_IRCClient.ActiveChannelSyncing = true;

            // Setup Events
            m_IRCClient.OnConnected += new EventHandler(ircClient_OnConnect);
            m_IRCClient.OnJoin += new JoinEventHandler(ircClient_OnJoin);
            m_IRCClient.OnNames += new NamesEventHandler(ircClient_OnNames);
            // Leave room
            m_IRCClient.OnPart += new PartEventHandler(ircClient_OnPart);
            m_IRCClient.OnQuit += new QuitEventHandler(ircClient_OnQuit);
            m_IRCClient.OnKick += new KickEventHandler(ircClient_OnKick);
            m_IRCClient.OnQueryMessage += new IrcEventHandler(ircClient_OnPrivMsg);
            m_IRCClient.OnErrorMessage += new IrcEventHandler(ircClient_OnError);
            //m_IRCClient.OnRawMessage += new IrcEventHandler(OnRawMessage);

            m_IRCClient.Connect(m_IRCServer, m_IRCPort);

            // here we logon and register our nickname and so on 
            m_IRCClient.Login(m_Nick, "SmartIrc4net Test Bot");

            // Setup listen thread
            m_IrcThread = new Thread(ListenThread);
            m_IrcThread.Start();


        }

        private void IRCMessenger_FormClosed(object sender, FormClosedEventArgs e)
        {
            // join the channel
            m_IRCClient.RfcQuit("Sign Off");
            m_IRCClient.Disconnect();
            m_IrcThread.Abort();

            // Close all messenger windows
            foreach (PrivateMessage PrivWin in m_PrivateWindows)
            {
                PrivWin.Close();
            }
        }           
    #endregion

        PrivateMessage FindWindow(String NickName)
        {
            foreach (PrivateMessage PrivWin in m_PrivateWindows)
            {
                if (PrivWin.m_PrivateNick == NickName)
                {
                    return PrivWin;
                }
            }
            return null;
        }

        public void SendMsg(String PrivateNick, String Text)
        {
            String[] Lines = Text.Split('\n');
            foreach (String Line in Lines)
            {
                m_IRCClient.RfcPrivmsg(PrivateNick, Line);
            }
        }

        void SendWindowMsg(IrcEventArgs e)
        {
            PrivateMessage Window = FindWindow(e.Data.Nick);

            if (Window != null && Window.m_Closing == true)
            {
                Window.Close();
                m_PrivateWindows.Remove(Window);
                Window = null;
            }

            if (Window == null)
            {
                Window = new PrivateMessage();
                Window.Show();
                Window.SetParent(this);
                Window.m_PrivateNick = e.Data.Nick;
                Window.m_MyNick = m_Nick;
                Window.Text = e.Data.Nick + " - EnB Messager";

                m_PrivateWindows.Add(Window);
            }

            if (Window != null && e.Data.Message != null)
            {
                Window.ReciveMessage(e.Data.Message);
            }
        }

    #region IRC Event Messages

        // Private message
        void ircClient_OnError(object sender, IrcEventArgs e)
        {
            if (e.Data.Message == "11")
            {
                MessageBox.Show("Player is not online anymore", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        void ircClient_OnPrivMsg(object sender, IrcEventArgs e)
        {
            BeginInvoke(new ActionDelegateIrcEvent(SendWindowMsg), e);
        }

        void ircClient_OnNames(object sender, NamesEventArgs e)
        {
            Channel Chan = m_IRCClient.GetChannel(m_IRCChannel);

            // join the channel
            m_UserTree.Nodes.Clear();
            foreach (DictionaryEntry de in Chan.Users)
            {
                ChannelUser channeluser = (ChannelUser)de.Value;
                // Don't show us on the list
                if (channeluser.Nick != m_Nick)
                {
                    TreeNode Tree = new TreeNode();
                    Tree.Text = channeluser.Nick;
                    m_UserTree.Nodes.Add(Tree);
                }
            }
            BeginInvoke(new ActionDelegateNoParams(UpdateUserList), null);
        }

        void ircClient_OnConnect(object sender, EventArgs e)
        {
            // join the channel
            m_IRCClient.RfcJoin(m_IRCChannel);
        }

        void ircClient_OnJoin(object sender, JoinEventArgs e)
        {
            if (e.Data.Nick != m_Nick)
            {
                m_IRCClient.RfcNames(e.Data.Channel);
                System.Console.WriteLine(e.Data.Nick + " Came Online");
            }
        }

        void ircClient_OnPart(object sender, PartEventArgs e)
        {
            if (e.Data.Nick != m_Nick)
            {
                m_IRCClient.RfcNames(e.Data.Channel);
                System.Console.WriteLine(e.Data.Nick + " went Offline");
            }
        }

        void ircClient_OnQuit(object sender, QuitEventArgs e)
        {
            if (e.Data.Nick != m_Nick)
            {
                m_IRCClient.RfcNames(m_IRCChannel);
                System.Console.WriteLine(e.Data.Nick + " went Offline");
            }
        }

        void ircClient_OnKick(object sender, KickEventArgs e)
        {
            if (e.Data.Nick != m_Nick)
            {
                m_IRCClient.RfcNames(e.Data.Channel);
                System.Console.WriteLine(e.Data.Nick + " went Offline");
            }
        }

        void OnRawMessage(object sender, IrcEventArgs e)
        {
            System.Console.WriteLine("Received: " + e.Data.RawMessage);
        }
    #endregion

    #region GUI Update Functions
        private void UpdateUserList()
        {
            // Update user list
            TreeNode []OnlineNode = UserList.Nodes.Find("Online", false);
            OnlineNode[0].Nodes.Clear();
            foreach(TreeNode Node in m_UserTree.Nodes)
            {
                OnlineNode[0].Nodes.Add((TreeNode) Node.Clone());
            }
            // Expand list
            OnlineNode[0].Expand();
        }
    #endregion

        private void UserList_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            IrcMessageData MsgData = new IrcMessageData(null, null, e.Node.Text, null, null, null, null, " ", (ReceiveType) 0, (ReplyCode) 0);
            IrcEventArgs Msg = new IrcEventArgs(MsgData);

            if (e.Node.Name != "Online")
            {
                SendWindowMsg(Msg);
            }
        }
    }
}
