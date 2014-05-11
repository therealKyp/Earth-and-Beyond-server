using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;

using Gajatko.IniFiles;

namespace EnBIniParser
{
    class BaseAssetParser
    {
        private int rslid = -1;

        public BaseAssetParser()
        {
            for (int i = 0; i < 2411; i++)
            {
                rslid = -1;
                try
                {
                    IniFileReader reader = new IniFileReader("C:\\Documents and Settings\\Josh\\Desktop\\basset.ini");
                    IniFileSectionStart test = reader.GotoSection("BASE-"+i);
                    List<IniFileValue> test2 = reader.ReadSectionValues();

                    foreach (IniFileValue r in test2)
                    {
                        if(r.Key == "RSLID")
                        {
                            rslid = int.Parse(r.Value.ToString());
                        }
                    }

                    int id = int.Parse(test.SectionName.Replace("BASE-", ""));

                    String query = "UPDATE assets SET rslid='"+rslid+"' where base_id='"+id+"';";

                    DataTable insert = Database.executeQuery(Database.DatabaseName.net7, query);
                    Console.Out.WriteLine(query);
                    reader.Close();
                }
                catch (Exception e)
                {
                    //Console.Out.WriteLine(e+", "+ e.Message);
                    //throw;
                }
            }

            Console.Out.WriteLine("***** Finished *****");
        }
    }
}
