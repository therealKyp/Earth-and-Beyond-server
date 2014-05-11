using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;

using Gajatko.IniFiles;

namespace EnBIniParser
{
    class BuffParser
    {
        private int id;
        private String type;
        private String asset_name;
        private int asset_id = -1;
        private String tooltip;
        private String alt_tooltip;
        private int is_good_buff;

        public BuffParser(int start, int end)
        {
            for (int i = start; i < end; i++)
            {
                try
                {
                    IniFileReader reader = new IniFileReader("C:\\Documents and Settings\\Josh\\Desktop\\buffdef.ini");
                    IniFileSectionStart test = reader.GotoSection("BUFF-"+i);
                    List<IniFileValue> test2 = reader.ReadSectionValues();

                    foreach (IniFileValue r in test2)
                    {
                        valueLookup(test, r);
                    }

                    id = int.Parse(test.SectionName.Replace("BUFF-", ""));

                    String query = "INSERT INTO buffs SET buff_id='" + id + "', type='"+type+"', "+
                        "asset_name='"+asset_name+"', asset_id='"+asset_id+"', tooltip='"+tooltip+"', "+
                        "alt_tooltip='"+alt_tooltip+"', is_good_buff='"+is_good_buff+"';";

                    DataTable buffInsert = Database.executeQuery(Database.DatabaseName.net7, query);
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

        private void valueLookup(IniFileSectionStart test, IniFileValue r)
        {
            String parsed = "";
            switch (r.Key.ToString())
            {
                case "BuffType":
                    parsed = r.Value.ToString();
                    type = parsed.Replace("'", "\\'");
                    break;
                case "BuffAsset":
                    parsed = r.Value.ToString();
                    asset_name = parsed.Replace("'", "\\'");
                    break;
                case "BuffToolTip":
                    parsed = r.Value.ToString();
                    tooltip = parsed.Replace("'", "\\'");
                    break;
                case "BuffAltToolTip":
                    parsed = r.Value.ToString();
                    alt_tooltip = parsed.Replace("'", "\\'");
                    break;
                case "IsGoodBuff":
                    bool boolTest = bool.Parse(r.Value.ToString());
                    is_good_buff = Convert.ToInt32(boolTest);
                    break;
            }
        }
    }
}
