using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CommonTools.Database;
using System.Data;
using CommonTools.Gui;
using System.Xml;

namespace MissionEditor.Database
{
    public class DataConfiguration
    {
        public enum DataType { faction, item, mob, sector_object, npc, sector, skill, mission };
        private Net7.Tables m_table;
        private Enum m_id;
        private Enum[] m_description;
        private static DlgSearch m_dlgSearch = new DlgSearch();

        private static Dictionary<DataConfiguration.DataType, DataConfiguration> m_DataConfigurations;

        public static void init()
        {
            m_DataConfigurations = new Dictionary<DataConfiguration.DataType, DataConfiguration>();
            m_DataConfigurations.Add(DataConfiguration.DataType.faction,
                                     new DataConfiguration(Net7.Tables.factions,
                                                           Net7.Table_factions._faction_id,
                                                           new Enum[] { Net7.Table_factions._name }));
            m_DataConfigurations.Add(DataConfiguration.DataType.item,
                                     new DataConfiguration(Net7.Tables.item_base,
                                                           Net7.Table_item_base._id,
                                                           new Enum[] { Net7.Table_item_base._name }));
            m_DataConfigurations.Add(DataConfiguration.DataType.mob,
                                     new DataConfiguration(Net7.Tables.mob_base,
                                                           Net7.Table_mob_base._mob_id,
                                                           new Enum[] { Net7.Table_mob_base._name }));
            m_DataConfigurations.Add(DataConfiguration.DataType.sector_object,
                                     new DataConfiguration(Net7.Tables.sector_objects,
                                                           Net7.Table_sector_objects._sector_object_id,
                                                           new Enum[] { Net7.Table_sector_objects._name })); //TODO: also used Net7.Table_sector_objects._sector_id's name for this item's description
            m_DataConfigurations.Add(DataConfiguration.DataType.npc,
                                     new DataConfiguration(Net7.Tables.starbase_npcs,
                                                           Net7.Table_starbase_npcs._npc_Id,
                                                           new Enum[] { Net7.Table_starbase_npcs._first_name, Net7.Table_starbase_npcs._last_name }));
            m_DataConfigurations.Add(DataConfiguration.DataType.sector,
                                     new DataConfiguration(Net7.Tables.sectors,
                                                           Net7.Table_sectors._sector_id,
                                                           new Enum[] { Net7.Table_sectors._name }));
            m_DataConfigurations.Add(DataConfiguration.DataType.skill,
                                     new DataConfiguration(Net7.Tables.skills,
                                                           Net7.Table_skills._skill_id,
                                                           new Enum[] { Net7.Table_skills._name }));
            m_DataConfigurations.Add(DataConfiguration.DataType.mission,
                                     new DataConfiguration(Net7.Tables.missions,
                                                           Net7.Table_missions._mission_id,
                                                           new Enum[] { Net7.Table_missions._mission_name }));
        }

        public static Boolean isValid(DataConfiguration.DataType dataType, String code)
        {
            DataConfiguration dataConfiguration;
            if (m_DataConfigurations.TryGetValue(dataType, out dataConfiguration))
            {
                return dataConfiguration.isValid(code);
            }
            else
            {
                throw (new Exception("Unable to convert '" + dataType.ToString() + "' into a DataConfiguration.DataType"));
            }
        }

        class DataValidation
        {
            public DataConfiguration dataConfiguration;
            public String code;
            public String errorMessage;
            public DataValidation(DataConfiguration dataConfiguration, String code, String errorMessage)
            {
                this.dataConfiguration = dataConfiguration;
                this.code = code;
                this.errorMessage = errorMessage;
            }
        }
        static List<DataValidation> listDataValidations = new List<DataValidation>();

        public static void addValidation(DataConfiguration.DataType dataType, String code)
        {
            DataConfiguration dataConfiguration;
            if (m_DataConfigurations.TryGetValue(dataType, out dataConfiguration))
            {
                String errorMessage = "The " + dataType.ToString() + " code '" + code + "' does not exist";
                listDataValidations.Add(new DataValidation(dataConfiguration, code, errorMessage));
            }
            else
            {
                throw (new Exception("Unable to convert '" + dataType.ToString() + "' into a DataConfiguration.DataType"));
            }
        }

        public static void addValidation(String errorMessage)
        {
            listDataValidations.Add(new DataValidation(null, null, errorMessage));
        }

        public static Boolean validate(out String errorMessage)
        {
            errorMessage = "";
            String query = "";
            int queryCount = 0;
            List<String> parameters = new List<string>();
            List<String> values = new List<string>();
            List<DataValidation> dbDataValidations = new List<DataValidation>();
            String parameter;

            foreach (DataValidation dataValidation in listDataValidations)
            {
                // Hack to allow non-database validations to feed into this system
                if ((dataValidation.dataConfiguration == null && dataValidation.code == null))
                {
                    errorMessage = dataValidation.errorMessage;
                    break;
                }

                parameter = dataValidation.dataConfiguration.m_id.ToString() + queryCount.ToString();
                if (query.Length != 0)
                {
                    query += " UNION ";
                }
                query += DB.SELECT
                       + queryCount.ToString()
                       + ","
                       + ColumnData.GetName(dataValidation.dataConfiguration.m_id)
                       + DB.FROM
                       + dataValidation.dataConfiguration.m_table
                       + DB.WHERE
                       + ColumnData.GetName(dataValidation.dataConfiguration.m_id)
                       + DB.EQUALS
                       + DB.QueryParameterCharacter
                       + parameter;
                parameters.Add(parameter);
                values.Add(dataValidation.code);
                dbDataValidations.Add(dataValidation);
                queryCount++;
            }

            if (errorMessage.Length == 0
                && query.Length != 0)
            {
                DataTable dataTable = DB.Instance.executeQuery(query, parameters.ToArray(), values.ToArray());
                for (int validation = 0; validation < dbDataValidations.Count; validation++)
                {
                    if (dataTable.Rows.Count <= validation
                        || !dataTable.Rows[validation][0].ToString().Equals(validation.ToString()))
                    {
                        errorMessage = dbDataValidations[validation].errorMessage;
                        break;
                    }
                }
            }
            listDataValidations.Clear();
            return errorMessage.Length == 0;
        }

        public static String search(DataType dataType)
        {
            DataConfiguration dataConfiguration;
            if (m_DataConfigurations.TryGetValue(dataType, out dataConfiguration))
            {
                m_dlgSearch.configure(dataConfiguration.m_table);
                m_dlgSearch.ShowDialog();
                return m_dlgSearch.getSelectedId();
            }
            else
            {
                throw (new Exception("Unable to convert '" + dataType.ToString() + "' into a DataConfiguration.DataType"));
            }
        }

        public static String getDescription(DataType dataType, String id)
        {
            DataConfiguration dataConfiguration;
            if (m_DataConfigurations.TryGetValue(dataType, out dataConfiguration))
            {
                String query = "";
                foreach (Enum field in dataConfiguration.m_description)
                {
                    if (query.Length == 0)
                    {
                        query += DB.SELECT;
                    }
                    else
                    {
                        query += ",";
                    }
                    query += ColumnData.GetName(field);
                }

                String parameter = "id";
                query += DB.FROM
                       + dataConfiguration.m_table.ToString()
                       + DB.WHERE
                       + ColumnData.GetName(dataConfiguration.m_id)
                       + DB.EQUALS
                       + DB.QueryParameterCharacter
                       + parameter;
                DataTable dataTable = DB.Instance.executeQuery(query, new String[] { parameter }, new String[] { id });
                String description = "";
                if (dataTable.Rows.Count == 1)
                {
                    foreach (DataColumn column in dataTable.Columns)
                    {
                        if (description.Length != 0)
                        {
                            description += " ";
                        }
                        description += dataTable.Rows[0][column].ToString();
                    }
                }
                return description + " (" + id + ")";
            }
            else
            {
                throw (new Exception("Unable to convert '" + dataType.ToString() + "' into a DataConfiguration.DataType"));
            }
        }

        public DataConfiguration(Net7.Tables table, Enum id, Enum[] description)
        {
            m_table = table;
            m_id = id;
            m_description = description;
        }

        public Boolean isValid(String code)
        {
            DataTable dataTable = DB.Instance.select(new Enum[] { m_id },
                                                     m_table,
                                                     m_id,
                                                     code);
            return dataTable.Rows.Count != 0;
        }

    }

}
