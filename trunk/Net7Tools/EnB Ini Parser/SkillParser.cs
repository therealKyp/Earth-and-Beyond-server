using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;

using Gajatko.IniFiles;

namespace EnBIniParser
{
    class SkillParser
    {
        private int skill_id;
        private String skill_name;
        private int is_activated;
        private String category;
        private String description;
        private String desc_1;
        private String desc_2;
        private String desc_3;
        private String desc_4;
        private String desc_5;
        private String desc_6;
        private String desc_7;
        private String desc_8;
        private String desc_9;
        private int warrior_max_level = -1;
        private int sentinel_max_level = -1;
        private int privateer_max_level = -1;
        private int defender_max_level = -1;
        private int explorer_max_level = -1;
        private int enforcer_max_level = -1;
        private int seeker_max_level = -1;
        private int scout_max_level = -1;
        private int tradesman_max_level = -1;

        public SkillParser()
        {
            IniFileReader reader = new IniFileReader("C:\\Documents and Settings\\Josh\\Desktop\\cskill_t.ini");
            IniFileSectionStart skillList = reader.GotoSection("All Skills");
            List<IniFileValue> skillListNames = reader.ReadSectionValues();

            foreach (IniFileValue r in skillListNames)
            {
                skill_id = int.Parse(r.Key.ToString());
                String parsedName = r.Value.ToString();
                skill_name = parsedName.Replace("'", "\\'");

                IniFileReader reader2 = new IniFileReader("C:\\Documents and Settings\\Josh\\Desktop\\cskill_t.ini");
                IniFileSectionStart skills = reader2.GotoSection(skill_name);
                List<IniFileValue> skillvalues = reader2.ReadSectionValues();

                foreach (IniFileValue r2 in skillvalues)
                {
                    valueLookup(skills, r2);
                }

                insertIntoDB();

                reader2.Close();
            }

            reader.Close();

            Console.Out.WriteLine("***** Finished *****");
        }

        private void valueLookup(IniFileSectionStart test, IniFileValue r)
        {
            String parsed = "";
            switch (r.Key.ToString())
            {
                case "Activated":
                    bool boolTest = bool.Parse(r.Value.ToString());
                    is_activated = Convert.ToInt32(boolTest);
                    break;
                case "Category":
                    parsed = r.Value.ToString();
                    category = parsed.Replace("'", "\\'");
                    break;
                case "Description":
                    parsed = r.Value.ToString();
                    description = parsed.Replace("'", "\\'");
                    break;
                case "Desc_1":
                    parsed = r.Value.ToString();
                    desc_1 = parsed.Replace("'", "\\'");
                    break;
                case "Desc_2":
                    parsed = r.Value.ToString();
                    desc_2 = parsed.Replace("'", "\\'");
                    break;
                case "Desc_3":
                    parsed = r.Value.ToString();
                    desc_3 = parsed.Replace("'", "\\'");
                    break;
                case "Desc_4":
                    parsed = r.Value.ToString();
                    desc_4 = parsed.Replace("'", "\\'");
                    break;
                case "Desc_5":
                    parsed = r.Value.ToString();
                    desc_5 = parsed.Replace("'", "\\'");
                    break;
                case "Desc_6":
                    parsed = r.Value.ToString();
                    desc_6 = parsed.Replace("'", "\\'");
                    break;
                case "Desc_7":
                    parsed = r.Value.ToString();
                    desc_7 = parsed.Replace("'", "\\'");
                    break;
                case "Desc_8":
                    parsed = r.Value.ToString();
                    desc_8 = parsed.Replace("'", "\\'");
                    break;
                case "Desc_9":
                    parsed = r.Value.ToString();
                    desc_9 = parsed.Replace("'", "\\'");
                    break;
                case "Range":
                    //range = Convert.ToInt32(r.Value.ToString());
                    break;
                case "Warrior":
                    warrior_max_level = Convert.ToInt32(r.Value.ToString());
                    break;
                case "Sentinel":
                    sentinel_max_level = Convert.ToInt32(r.Value.ToString());
                    break;
                case "Privateer":
                    privateer_max_level = Convert.ToInt32(r.Value.ToString());
                    break;
                case "Defender":
                    defender_max_level = Convert.ToInt32(r.Value.ToString());
                    break;
                case "Explorer":
                    explorer_max_level = Convert.ToInt32(r.Value.ToString());
                    break;
                case "Seeker":
                    seeker_max_level = Convert.ToInt32(r.Value.ToString());
                    break;
                case "Enforcer":
                    enforcer_max_level = Convert.ToInt32(r.Value.ToString());
                    break;
                case "Scout":
                    scout_max_level = Convert.ToInt32(r.Value.ToString());
                    break;
                case "Tradesman":
                    tradesman_max_level = Convert.ToInt32(r.Value.ToString());
                    break;
            }
        }

        private void insertIntoDB()
        {
            //Insert Skills
           String skillsQuery = "INSERT INTO skills SET skill_id='" + skill_id + "', name='" + skill_name + "', " +
                "description='" + description + "', is_activated='" + is_activated + "', category='" + category + "', "+
                "warrior_max_level='"+warrior_max_level+"', sentinal_max_level='"+sentinel_max_level+"', "+
                "privateer_max_level='"+privateer_max_level+"', defender_max_level='"+defender_max_level+"', "+
                "explorer_max_level='"+explorer_max_level+"', seeker_max_level='"+seeker_max_level+"', "+
                "enforcer_max_level='"+enforcer_max_level+"', scout_max_level='"+scout_max_level+"', "+
                "tradesman_max_level='"+tradesman_max_level+"';";
            DataTable skillInsert = Database.executeQuery(Database.DatabaseName.net7, skillsQuery);

            for (int i = 1; i < 10; i++)
            {
                String level_desc = "";
                switch (i)
                {
                    case 1:
                        level_desc = desc_1;
                        break;
                    case 2:
                        level_desc = desc_2;
                        break;
                    case 3:
                        level_desc = desc_3;
                        break;
                    case 4:
                        level_desc = desc_4;
                        break;
                    case 5:
                        level_desc = desc_5;
                        break;
                    case 6:
                        level_desc = desc_6;
                        break;
                    case 7:
                        level_desc = desc_7;
                        break;
                    case 8:
                        level_desc = desc_8;
                        break;
                    case 9:
                        level_desc = desc_9;
                        break;
                }

                //Insert Skill Levels
                String skillLevelQuery = "INSERT INTO skill_levels SET skill_id='" + skill_id + "', " +
                    "level='" + i + "', description='" + level_desc + "';";

                DataTable skillLevelInsert = Database.executeQuery(Database.DatabaseName.net7, skillLevelQuery);
            }

            Console.Out.WriteLine("Inserting SkillID: "+skill_id+", SkillName: "+skill_name);
        }
    }
}
