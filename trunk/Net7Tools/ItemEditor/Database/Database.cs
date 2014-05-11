using System;
using System.Collections.Generic;
using System.Text;
using MySql.Data.MySqlClient;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;
using CommonTools.Database;

// http://www.csharp-station.com/Tutorials/AdoDotNet/Lesson01.aspx

namespace Net7_Tools.Database
{
    static class Database
    {

        public static String getNextItemId()
        {
            String query = "SELECT MAX("
                         + ColumnData.GetName(Net7.Table_item_base._id)
                         + ") FROM "
                         + Net7.Tables.item_base.ToString();
            DataTable dataTable = DB.Instance.executeQuery(query, null, null);
            Int32 nextId = 1;
            if (dataTable.Rows.Count == 1)
            {
                DataRow dataRow = dataTable.Rows[0];
                nextId = Int32.Parse(dataRow[0].ToString()) + 1;
            }
            return nextId.ToString();
        }

        public static List<String> getEveryItem()
        {
            List<String> items = new List<String>();
            String query = "SELECT "
                         + ColumnData.GetName(Net7.Table_item_base._id)
                         + " FROM "
                         + Net7.Tables.item_base.ToString()
                         + " ORDER BY "
                         + ColumnData.GetName(Net7.Table_item_base._id);
            DataTable dataTable = DB.Instance.executeQuery(query, null, null);
            foreach (DataRow dataRow in dataTable.Rows)
            {
                items.Add(ColumnData.GetString(dataRow, Net7.Table_item_base._id));
            }
            return items;
        }

        public static CodeValue[] getManufacturers()
        {
            List<CodeValue> codeValueList = new List<CodeValue>();
            String query = "SELECT "
                         + ColumnData.GetName(Net7.Table_item_manufacturer_base._id) + ", "
                         + ColumnData.GetName(Net7.Table_item_manufacturer_base._name)
                         + " FROM "
                         + Net7.Tables.item_manufacturer_base.ToString()
                         + " ORDER BY "
                         + ColumnData.GetName(Net7.Table_item_manufacturer_base._name);

            DataTable dataTable = DB.Instance.executeQuery(query, null, null);
            foreach (DataRow dataRow in dataTable.Rows)
            {
                codeValueList.Add(CodeValue.Formatted(ColumnData.GetInt32(dataRow, Net7.Table_item_manufacturer_base._id),
                                                       ColumnData.GetString(dataRow, Net7.Table_item_manufacturer_base._name)));
            }
            return codeValueList.ToArray();
        }

        public static CodeValue[] getItemCategories()
        {
            List<CodeValue> codeValueList = new List<CodeValue>();

            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Invalid, "Invalid"));

            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Ammo_Component, "Ammo Component"));
            //codeValueList.Add(CodeValue.Formatted((int) Items.ItemCategory.Consumable, "Consumable"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Core_Item, "Core Item"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Device, "Device"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Electronic_Item, "Electronic Item"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Fabricated_Item, "Fabricated Item"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Looted_Item, "Looted Item"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Raw_Resource, "Raw Resource"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Reactor_Component, "Reactor Component"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Refined_Resource, "Refined Resource"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Trade_Good, "Trade Good"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Weapon, "Weapon"));
            codeValueList.Add(CodeValue.Formatted((int)Items.ItemCategory.Weapon_Component, "Weapon Component"));

            return codeValueList.ToArray();
        }

        public static CodeValue[] getItemStatus()
        {
            List<CodeValue> codeValueList = new List<CodeValue>();

            codeValueList.Add(CodeValue.Formatted(0, "Incomplete data"));
            codeValueList.Add(CodeValue.Formatted(1, "Unverified"));
            codeValueList.Add(CodeValue.Formatted(2, "Verified by 1"));
            codeValueList.Add(CodeValue.Formatted(3, "Verified by 2"));
            codeValueList.Add(CodeValue.Formatted(4, "Verified by 3"));

            return codeValueList.ToArray();
        }

        public static CodeValue[] getItemTypes()
        {
            List<CodeValue> valueList = new List<CodeValue>();
            String query = "SELECT "
                 + ColumnData.GetName(Net7.Table_item_type._id) + ", "
                 + ColumnData.GetName(Net7.Table_item_type._name)
                 + " FROM "
                 + Net7.Tables.item_type.ToString()
                 + " ORDER BY "
                 + ColumnData.GetName(Net7.Table_item_type._name);
            DataTable dataTable = DB.Instance.executeQuery(query, null, null);
            foreach (DataRow dataRow in dataTable.Rows)
            {
                valueList.Add(CodeValue.Formatted(ColumnData.GetInt32(dataRow, Net7.Table_item_type._id),
                                                  ColumnData.GetString(dataRow, Net7.Table_item_type._name)));
            }
            return valueList.ToArray();
        }

        public static List<CodeValue> getItems(String category, String subCategory)
        {
            List<CodeValue> valueList = new List<CodeValue>();
            String categoryColumn = ColumnData.GetName(Net7.Table_item_base._category);
            String subCategoryColumn = ColumnData.GetName(Net7.Table_item_base._sub_category);
            String query = "SELECT "
                 + ColumnData.GetName(Net7.Table_item_base._id) + ", "
                 + ColumnData.GetName(Net7.Table_item_base._name)
                 + " FROM "
                 + Net7.Tables.item_base.ToString()
                 + " WHERE "
                 + categoryColumn
                 + " = "
                 + DB.QueryParameterCharacter + categoryColumn
                 + " AND "
                 + subCategoryColumn
                 + " = "
                 + DB.QueryParameterCharacter + subCategoryColumn
                 + " ORDER BY "
                 + ColumnData.GetName(Net7.Table_item_base._name);
            DataTable dataTable = DB.Instance.executeQuery(query,
                                                           new String[] { categoryColumn, subCategoryColumn },
                                                           new String[] { category, subCategory });
            CodeValue codeValue;
            foreach (DataRow dataRow in dataTable.Rows)
            {
                codeValue = new CodeValue(ColumnData.GetInt32(dataRow, Net7.Table_item_base._id),
                                           ColumnData.GetString(dataRow, Net7.Table_item_base._name));
                valueList.Add(codeValue);

            }
            return valueList;
        }

        public static CodeValue[] getItemSubCategories()
        {
            List<CodeValue> codeValueList = new List<CodeValue>();
            foreach (Items.ItemSubCategory subCategory in Enum.GetValues(typeof(Items.ItemSubCategory)))
            {
                codeValueList.Add(CodeValue.Formatted((int)subCategory, Items.SubCategoryLookup(subCategory)));
            }
            return codeValueList.ToArray();
        }

        public static CodeValue[] getItemSubCategories(int itemCategory)
        {
            List<CodeValue> codeValueList = new List<CodeValue>();

            switch ((Items.ItemCategory)itemCategory)
            {
                case Items.ItemCategory.Weapon:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Ammo, "Ammo"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Beam_Weapon, "Beam Weapon"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Missile_Launcher, "Missile Launcher"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Projectile_Launcher, "Projectile Launcher"));
                    break;

                case Items.ItemCategory.Device:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Device, "Device"));
                    break;

                case Items.ItemCategory.Core_Item:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Engine, "Engine"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Reactor, "Reactor"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Shield, "Shield"));
                    break;

                case Items.ItemCategory.Electronic_Item:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Electronic_Item, "Electronic Item"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Computer, "Computer"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Software, "Software"));
                    break;

                case Items.ItemCategory.Reactor_Component:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Power_Converter, "Power Converter"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Power_Core, "Power Core"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Power_Coupling, "Power Coupling"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Power_Generator, "Power Generator"));
                    break;

                case Items.ItemCategory.Fabricated_Item:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Casing, "Casing"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Drone_Frame, "Drone Frame"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Engine_Frame, "Engine Frame"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Mount, "Mount"));
                    break;

                case Items.ItemCategory.Weapon_Component:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Ammunition_Feeder, "Ammunition Feeder"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Beam_Optic, "Beam Optic"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Firing_Mechanism, "Firing Mechanism"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Weapon_Barrel, "Weapon Barrel"));
                    break;

                case Items.ItemCategory.Ammo_Component:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Ammunition_Slug, "Ammunition Slug"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Propellant, "Propellant"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Shell_Casing, "Shell Casing"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Warhead, "Warhead"));
                    break;

                case Items.ItemCategory.Raw_Resource:
                case Items.ItemCategory.Refined_Resource:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Alloy, "Alloy"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Catalyst, "Catalyst"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Core_Metal, "Core Metal"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Conductor, "Conductor"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Gas, "Gas"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Gemstone, "Gemstone"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Hydrocarbon, "Hydrocarbon"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Magnetic, "Magnetic"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Metal, "Metal"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Optic_Gem, "Optic Gem"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Radioactive, "Radioactive"));
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Silicate, "Silicate"));
                    break;

                case Items.ItemCategory.Trade_Good:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Other, "Other"));
                    break;

                case Items.ItemCategory.Looted_Item:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Other, "Other"));
                    break;

                default:
                    codeValueList.Add(CodeValue.Formatted((int)Items.ItemSubCategory.Invalid, "--= Invalid Sub-Category =--"));
                    break;
            }
            return codeValueList.ToArray();
        }

        public static Boolean getItemCategorySubcategory(String itemId, out String category, out String subCategory)
        {
            String itemColumn = ColumnData.GetName(Net7.Table_item_base._id);
            String query = "SELECT "
                 + ColumnData.GetName(Net7.Table_item_base._category) + ", "
                 + ColumnData.GetName(Net7.Table_item_base._sub_category)
                 + " FROM "
                 + Net7.Tables.item_base.ToString()
                 + " WHERE "
                 + itemColumn
                 + " = "
                 + DB.QueryParameterCharacter + itemColumn;
            DataTable dataTable = DB.Instance.executeQuery(query,
                                                           new String[] { itemColumn },
                                                           new String[] { itemId });
            if(dataTable.Rows.Count == 1)
            {
                DataRow dataRow = dataTable.Rows[0];
                category = ColumnData.GetString(dataRow, Net7.Table_item_base._category);
                subCategory = ColumnData.GetString(dataRow, Net7.Table_item_base._sub_category);
                return true;
            }

            category = "";
            subCategory = "";
            return false;
        }

        public static CodeValue[] getItemDamageTypes()
        {
            List<CodeValue> codeValueList = new List<CodeValue>();

            codeValueList.Add(CodeValue.Formatted(6, "Absolute"));
            codeValueList.Add(CodeValue.Formatted(5, "Chemical"));
            codeValueList.Add(CodeValue.Formatted(8, "Effect"));
            codeValueList.Add(CodeValue.Formatted(4, "EMP"));
            codeValueList.Add(CodeValue.Formatted(3, "Energy"));
            codeValueList.Add(CodeValue.Formatted(1, "Explosive"));
            codeValueList.Add(CodeValue.Formatted(0, "Impact"));
            codeValueList.Add(CodeValue.Formatted(2, "Plasma"));
            codeValueList.Add(CodeValue.Formatted(7, "System"));

            return codeValueList.ToArray();
        }

        public static String getQueryItem()
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_base.ToString();
        }

        public static String getQueryItem(String itemId)
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_base.ToString()
                 + " WHERE "
                 + ColumnData.GetName(Net7.Table_item_base._id)
                 + " = '"
                 + itemId
                 + "'";
        }

        public static String getQueryItem_param()
        {
            String idColumn = ColumnData.GetName(Net7.Table_item_base._id);
            return "SELECT * FROM "
                 + Net7.Tables.item_base.ToString()
                 + " WHERE "
                 + idColumn
                 + " = "
                 + DB.QueryParameterCharacter + idColumn;
        }

        public static String getQueryItemCategory()
        {
            return "true";
            // TODO: Create the item_category table
            /*return "SELECT *"
                 + " FROM " + Net7_db.Tables.item_category.ToString();*/
        }

        public static String getQueryItemSubCategory(int categoryId)
        {
            return "true";
            // TODO: Create the item_sub_category table
            /*return "SELECT *"
                 + " FROM " + Net7_db.Tables.item_sub_category.ToString()
                 + " WHERE " + ColumnData.GetName(Net7_db.item_sub_category._category_id)
                 + " = '" + categoryId + "'";*/
        }

        public static String getQueryMissile(String itemId)
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_missile.ToString()
                 + " WHERE "
                 + ColumnData.GetName(Net7.Table_item_missile._item_id)
                 + " = '"
                 + itemId
                 + "'";
        }

        public static String getQueryProjectile(String itemId)
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_projectile.ToString()
                 + " WHERE "
                 + ColumnData.GetName(Net7.Table_item_projectile._item_id)
                 + " = '"
                 + itemId
                 + "'";
        }

        public static String getQueryAmmo()
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_ammo.ToString();
        }

        public static String getQueryAmmo(String itemId)
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_ammo.ToString()
                 + " WHERE "
                 + ColumnData.GetName(Net7.Table_item_ammo._item_id)
                 + " = '"
                 + itemId
                 + "'";
        }

        public static String getQueryBeam()
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_beam.ToString();
        }

        public static String getQueryBeam(String itemId)
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_beam.ToString()
                 + " WHERE "
                 + ColumnData.GetName(Net7.Table_item_beam._item_id)
                 + " = '"
                 + itemId
                 + "'";
        }

        public static String getQueryProjectile()
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_projectile.ToString();
        }

        public static String getQueryMissile()
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_missile.ToString();
        }

        public static String getQueryDevice()
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_device.ToString();
        }

        public static String getQueryEngine()
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_engine.ToString();
        }

        public static String getQueryReactor()
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_reactor.ToString();
        }

        public static String getQueryShield()
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_shield.ToString();
        }

        public static CodeValue[] getItemManufacturingLevels()
        {
            List<CodeValue> codeValueList = new List<CodeValue>();
            // Okeanos: http://forum.enb-emulator.com/index.php?showtopic=4164&st=0&#
            codeValueList.Add(CodeValue.Formatted(1, "Automatic"));
            codeValueList.Add(CodeValue.Formatted(2, "Easy"));
            codeValueList.Add(CodeValue.Formatted(3, "Moderate"));
            codeValueList.Add(CodeValue.Formatted(4, "Hard"));
            codeValueList.Add(CodeValue.Formatted(5, "VeryHard"));
            codeValueList.Add(CodeValue.Formatted(6, "Impossible"));
            return codeValueList.ToArray();
        }

        public static CodeValue[] getItemAmmoTypes(Int32 subCategory)
        {
            List<CodeValue> codeValueList = new List<CodeValue>();
            if (subCategory == 100)
            {
                // Override the ammo type for beam weapons
                codeValueList.Add(CodeValue.Formatted(4, "EMP"));
                codeValueList.Add(CodeValue.Formatted(3, "Energy"));
                codeValueList.Add(CodeValue.Formatted(2, "Plasma"));
            }
            else
            {
                String query = "SELECT "
                             + ColumnData.GetName(Net7.Table_item_type._id) + ", "
                             + ColumnData.GetName(Net7.Table_item_type._name)
                             + " FROM "
                             + Net7.Tables.item_ammo_type.ToString()
                             + " WHERE "
                             + ColumnData.GetName(Net7.Table_item_ammo_type._sub_category)
                             + " = "
                             + subCategory
                             + " ORDER BY "
                             + ColumnData.GetName(Net7.Table_item_type._name);
                DataTable dataTable = DB.Instance.executeQuery(query, null, null);
                foreach (DataRow dataRow in dataTable.Rows)
                {
                    codeValueList.Add(CodeValue.Formatted(ColumnData.GetInt32(dataRow, Net7.Table_item_type._id),
                                                           ColumnData.GetString(dataRow, Net7.Table_item_type._name)));

                }
            }
            return codeValueList.ToArray();
        }

        public static String getItemTypeQuery()
        {
            String query = "SELECT * FROM "
                         + Net7.Tables.item_type.ToString();
            return query;
        }

        public static String getQueryManufacturing()
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_manufacture.ToString();
        }

        public static Boolean hasManufacturing(String itemId)
        {
            String itemIdColumn = ColumnData.GetName(Net7.Table_item_manufacture._item_id);
            String query = "SELECT "
                         + itemIdColumn
                         + " FROM "
                         + Net7.Tables.item_manufacture.ToString()
                         + " WHERE "
                         + itemIdColumn
                         + " = "
                         + DB.QueryParameterCharacter + itemIdColumn
                         + " LIMIT 1";
            DataTable dataTable = DB.Instance.executeQuery(query, new String[] { itemIdColumn }, new String[] { itemId });
            return dataTable.Rows.Count != 0;
        }

        public static String[] getAssetCategories()
        {
            List<String> valueList = new List<String>();
            String query = "SELECT DISTINCT "
                         + ColumnData.GetName(Net7.Table_assets._main_cat)
                         + " FROM "
                         + Net7.Tables.assets.ToString()
                         + " ORDER BY "
                         + ColumnData.GetName(Net7.Table_assets._main_cat);
            DataTable dataTable = DB.Instance.executeQuery(query, null, null);
            foreach (DataRow dataRow in dataTable.Rows)
            {
                valueList.Add(ColumnData.GetString(dataRow, Net7.Table_assets._main_cat));

            }
            return valueList.ToArray();
        }

        public static String[] getAssetSubCategories(String categoryValue)
        {
            List<String> valueList = new List<String>();
            String categoryColumn = ColumnData.GetName(Net7.Table_assets._main_cat);
            String query = "SELECT DISTINCT "
                         + ColumnData.GetName(Net7.Table_assets._sub_cat)
                         + " FROM "
                         + Net7.Tables.assets.ToString()
                         + " WHERE "
                         + categoryColumn
                         + " = "
                         + DB.QueryParameterCharacter + categoryColumn
                         + " ORDER BY "
                         + ColumnData.GetName(Net7.Table_assets._sub_cat);
            DataTable dataTable = DB.Instance.executeQuery(query, new String[] { categoryColumn }, new String[] { categoryValue });
            foreach (DataRow dataRow in dataTable.Rows)
            {
                valueList.Add(ColumnData.GetString(dataRow, Net7.Table_assets._sub_cat));

            }
            return valueList.ToArray();
        }

        public static AssetInformation[] getAssetItems(String categoryValue, String subCategoryValue)
        {
            String categoryColumn = ColumnData.GetName(Net7.Table_assets._main_cat);
            String subCategoryColumn = ColumnData.GetName(Net7.Table_assets._sub_cat);
            String[] parameters;
            String[] values;
            List<AssetInformation> valueList = new List<AssetInformation>();
            String query = "SELECT  "
                         + ColumnData.GetName(Net7.Table_assets._base_id) + ", "
                         + ColumnData.GetName(Net7.Table_assets._descr) + ", "
                         + ColumnData.GetName(Net7.Table_assets._filename)
                         + " FROM "
                         + Net7.Tables.assets.ToString()
                         + " WHERE "
                         + categoryColumn
                         + " = "
                         + DB.QueryParameterCharacter + categoryColumn;
            if (subCategoryValue == null)
            {
                parameters = new String[] { categoryColumn };
                values = new String[] { categoryValue };
            }
            else
            {
                query += " AND "
                      + subCategoryColumn
                      + " = "
                      + DB.QueryParameterCharacter + subCategoryColumn;
                parameters = new String[] { categoryColumn, subCategoryColumn };
                values = new String[] { categoryValue, subCategoryValue };
            }
            query += " ORDER BY "
                  +  ColumnData.GetName(Net7.Table_assets._filename);
            DataTable dataTable = DB.Instance.executeQuery(query, parameters, values);
            AssetInformation assetInformation;
            foreach (DataRow dataRow in dataTable.Rows)
            {
                assetInformation = new AssetInformation();
                assetInformation.base_id = ColumnData.GetInt32(dataRow, Net7.Table_assets._base_id);
                assetInformation.descr = ColumnData.GetString(dataRow, Net7.Table_assets._descr);
                assetInformation.filename = ColumnData.GetString(dataRow, Net7.Table_assets._filename);
                valueList.Add(assetInformation);
            }
            return valueList.ToArray();
        }

        public static AssetInformation getAsset(String assetId)
        {
            String idColumn = ColumnData.GetName(Net7.Table_assets._base_id);
            String query = "SELECT  "
                         + ColumnData.GetName(Net7.Table_assets._base_id) + ", "
                         + ColumnData.GetName(Net7.Table_assets._descr) + ", "
                         + ColumnData.GetName(Net7.Table_assets._filename)
                         + " FROM "
                         + Net7.Tables.assets.ToString()
                         + " WHERE "
                         + idColumn
                         + " = "
                         + DB.QueryParameterCharacter + idColumn;
            DataTable dataTable = DB.Instance.executeQuery(query, new String[] { idColumn }, new String[] { assetId });
            AssetInformation assetInformation = null;
            if (dataTable.Rows.Count == 1)
            {
                DataRow dataRow = dataTable.Rows[0];
                assetInformation = new AssetInformation();
                assetInformation.base_id = ColumnData.GetInt32(dataRow, Net7.Table_assets._base_id);
                assetInformation.descr = ColumnData.GetString(dataRow, Net7.Table_assets._descr);
                assetInformation.filename = ColumnData.GetString(dataRow, Net7.Table_assets._filename);
            }
            return assetInformation;
        }

        static public String getAssetFromDescription(String description)
        {
            String descColumn = ColumnData.GetName(Net7.Table_assets._descr);
            String query = "SELECT "
                         + ColumnData.GetName(Net7.Table_assets._base_id)
                         + " FROM "
                         + Net7.Tables.assets.ToString()
                         + " WHERE "
                         + descColumn
                         + " = "
                         + DB.QueryParameterCharacter + descColumn;
            DataTable dataTable = DB.Instance.executeQuery(query, new String[] { descColumn }, new String[] { description });
            String assetId = "";
            if (dataTable.Rows.Count == 1)
            {
                DataRow dataRow = dataTable.Rows[0];
                assetId = ColumnData.GetString(dataRow, Net7.Table_assets._base_id);
            }
            return assetId;
        }

        static public Boolean getAssetCatSubCatFromId(String id, out String category, out String subCategory)
        {
            String idColumn = ColumnData.GetName(Net7.Table_assets._base_id); 
            String query = "SELECT "
                         + ColumnData.GetName(Net7.Table_assets._main_cat)
                         + ", "
                         + ColumnData.GetName(Net7.Table_assets._sub_cat)
                         + " FROM "
                         + Net7.Tables.assets.ToString()
                         + " WHERE "
                         + idColumn
                         + " = "
                         + DB.QueryParameterCharacter + idColumn;
            DataTable dataTable = DB.Instance.executeQuery(query, new String[] { idColumn }, new String[] { id });
            if (dataTable.Rows.Count == 1)
            {
                DataRow dataRow = dataTable.Rows[0];
                category = ColumnData.GetString(dataRow, Net7.Table_assets._main_cat);
                subCategory = ColumnData.GetString(dataRow, Net7.Table_assets._sub_cat);
                return true;
            }
            else
            {
                category = "";
                subCategory = "";
                return false;
            }
        }

        static public String getVersion()
        {
            String query = "SELECT "
                         + ColumnData.GetName(Net7.Table_versions._Version)
                         + " FROM "
                         + Net7.Tables.versions.ToString()
                         + " WHERE "
                         + ColumnData.GetName(Net7.Table_versions._EName)
                         + " = 'Item'";
            DataTable dataTable = DB.Instance.executeQuery(query, null, null);
            return ColumnData.GetString(dataTable.Rows[0], Net7.Table_versions._Version);
        }

    }
}
