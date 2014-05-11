using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace TalkTreeEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(String[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            String conversation = "<Tree Node=\"1\">"
                                + "<Text>/angry I take it you are here for the phrase? </Text>"
                                + "<Branch Node=\"2\">Not I!</Branch>"
                                + "<Branch Node=\"3\">Yes Sir!</Branch>"
                                + "</Tree>"

                                + "<Tree Node=\"2\">"
                                + "<Text>/crazy Then what are you doing wasting my time?</Text>"
                                + "</Tree>"

                                + "<Tree Node=\"3\">"
                                + "<Text>/bow Are you sure you want to keep playing? </Text>"
                                + "<Branch Node=\"4\">No, I don't!</Branch>"
                                + "<Branch Node=\"5\">SIR, YES SIR!</Branch>"
                                + "</Tree>"

                                + "<Tree Node=\"4\">"
                                + "<Text>/crazy Quitter</Text>"
                                + "</Tree>"

                                + "<Tree Node=\"5\">"
                                + "<Text>/give \"than\", and now you must see Executive Mackellan, hope you remember where she is. </Text>"
                                + "</Tree>";

            conversation = "<Tree Node=\"1\">"
                         + "<Text>/bow Greeting, I am Roger, and I have some Equiptment for you!</Text>"
                         + "<Branch Node=\"2\">Trade!</Branch>"
                         + "<Branch Node=\"3\">No Thanks!</Branch>"
                         + "</Tree>"

                         + "<Tree Node=\"2\">"
                         + "<Text>/give Enjoy!</Text>"
                         + "<Trade>1</Trade>"
                         + "</Tree>"

                         + "<Tree Node=\"3\">"
                         + "<Text>/bow Maybe next time.</Text>"
                         + "</Tree>";

            conversation = "<Tree Node=\"1\">"
                         + "<Text>un</Text>"
                         + "<Branch Node=\"2\">goto2</Branch>"
                         + "</Tree>"
                         + "<Tree Node=\"2\">"
                         + "<Text>trade</Text>"
                         + "<Trade></Trade>"
                         + "<Flags>1</Flags>"
                         + "</Tree>"
                         + "<Tree Node=\"3\">"
                         + "<Text>drop mission</Text>"
                         + "<Flags>3</Flags>"
                         + "</Tree>"
                         + "<Tree Node=\"4\">"
                         + "<Text>completed</Text>"
                         + "<Flags>6</Flags>"
                         + "</Tree>"
                         + "<Tree Node=\"5\">"
                         + "<Text>goto stage</Text>"
                         + "<Flags Data=\"0\">5</Flags>"
                         + "</Tree>"
                         + "<Tree Node=\"6\">"
                         + "<Text>more</Text>"
                         + "<Flags>4</Flags>"
                         + "</Tree>"
                         + "<Tree Node=\"7\">"
                         + "<Text>postpone</Text>"
                         + "<Flags>2</Flags>"
                         + "</Tree>";

            if (args != null && args.Length != 0)
            {
                conversation = args[0];
            }

            FrmTalkTree frmTalkTree = new FrmTalkTree();
            frmTalkTree.setConversation(conversation);
            //Application.Run(new FrmTalkTree());
            //Application.Run(frmTalkTree);
            frmTalkTree.ShowDialog();
            if (frmTalkTree.getConversation(out conversation))
            {
                System.Console.WriteLine("\n\n" + conversation + "\n\n");
                if (args != null && args.Length != 0)
                {
                    Clipboard.SetText(conversation);
                }
            }

        }
    }
}
