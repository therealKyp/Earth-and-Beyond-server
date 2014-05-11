using System;
using System.Data;
using System.Windows.Forms;
using System.Xml;

namespace Net7_Tools
{
	static class Program
	{
        enum ApplicationAction { GenerateDatabaseStructure, UpdateVersion, Run, Search };

		[STAThread]
		static void Main()
		{
            //new Test(); return;

			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);

            ApplicationAction applicationAction = ApplicationAction.Run;

            CommonTools.Gui.Login login = new CommonTools.Gui.Login();
            if (applicationAction.Equals(ApplicationAction.UpdateVersion))
            {
                login.updateVersion();
            }
            login.ShowDialog();
            if (login.isValid())
            {
                switch (applicationAction)
                {
                    case ApplicationAction.GenerateDatabaseStructure:
                        CommonTools.Database.DB.Instance.makeDatabaseVariables();
                        break;
                    case ApplicationAction.Run:
                        Application.Run(new frmItems()); // Start the main application
                        break;
                }
                //Search.frmAsset.convertImages(); // Convert the original assets
                //Application.Run(new Search.frmAsset()); // Shortcut to the asset search dialog
                //frmItemTree itemTree = new frmItemTree(); itemTree.setSelectedItemId("516"); Application.Run(itemTree); // Shortcut to the item search dialog
            }
		}
	}
}