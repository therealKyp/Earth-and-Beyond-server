using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using MySql.Data.MySqlClient;
using Net7_Tools.Database;
using CommonTools.Database;

namespace Net7_Tools
{
/*
    class Test
    {
        private MySqlConnection conn;
	    private MySqlDataAdapter sqlDataAdapter;
	    private DataSet  dataSet;
        private int m_testCount = 0;

        public Test()
        {
            conn = new MySqlConnection(SQLData.ConnStr());
            //test_parameterQuery();
            //test_queryPerformance();
            //test_dataSet();
            //test_dataValidation();
            test_dataType();
        }

        private void test_dataType()
        {
            Console.WriteLine("item_base.2d_asset (int == "
                             + ColumnData.GetDataType(Net7.Table_item_base._2d_asset)
                             + ") is a String: "
                             + ColumnData.IsDataTypeString(Net7.Table_item_base._2d_asset));
            Console.WriteLine("item_base.name is (varchar == "
                             + ColumnData.GetDataType(Net7.Table_item_base._name)
                             + ") is a String: "
                             + ColumnData.IsDataTypeString(Net7.Table_item_base._name));
        }

        private void test_dataValidation()
        {
            ValidateNumericGreaterEqual validateNumericGreaterEqual = new ValidateNumericGreaterEqual(new Decimal(0));
            validate("ValidateNumericGreaterEqual(0)", validateNumericGreaterEqual.validate("0"));
            validate("ValidateNumericGreaterEqual(0)", validateNumericGreaterEqual.validate("1"));
            validate("ValidateNumericGreaterEqual(0)", validateNumericGreaterEqual.validate("0.1"));

            System.Console.WriteLine("***Performed " + m_testCount.ToString() + " tests");
        }

        private void validate(String test, DataValidation.Result result)
        {
            ++m_testCount;
            if (!result.isValid())
            {
                System.Console.WriteLine(test + ": " + result.getMessage());
            }
        }

        public void test_dataSet()
        {
            // Initialize the DataSet with a single (first) record
            MySqlConnection myConnection = new MySqlConnection(SQLData.ConnStr());
            String query = "SELECT * FROM item_base LIMIT 1";
            MySqlDataAdapter m_sqlDataAdapter = new MySqlDataAdapter(query, myConnection);

            // Set the MissingSchemaAction property to AddWithKey because Fill will not cause primary
            // key & unique key information to be retrieved unless AddWithKey is specified.
            m_sqlDataAdapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;

            //m_sqlDataAdapter.DeleteCommand = new MySqlCommand("DELETE FROM item_base WHERE id = 

            // Fill-in the insert, update, and delete commands
            //MySqlCommandBuilder mySqlCommandBuilder = new MySqlCommandBuilder(m_sqlDataAdapter);
            //Console.WriteLine("Delete: " + mySqlCommandBuilder.GetDeleteCommand().CommandText);
            //Console.WriteLine("Insert: " + mySqlCommandBuilder.GetInsertCommand().CommandText);
            //Console.WriteLine("Update: " + mySqlCommandBuilder.GetUpdateCommand().CommandText);

            DataSet m_dataSet = new DataSet();
            m_sqlDataAdapter.Fill(m_dataSet, "item_base");
            m_sqlDataAdapter.Update(m_dataSet, "item_base");
            DB.printDataSetContents(m_dataSet, "test");

            // Search the DataSet for a particular record. If not present then retrieve from DB
            DataRow[] dataRows = m_dataSet.Tables["item_base"].Select("id = 4060");
            if (dataRows.Length == 0)
            {
                query = "SELECT * FROM item_base WHERE id = 4060";
                m_sqlDataAdapter = new MySqlDataAdapter(query, myConnection);
                m_sqlDataAdapter.Fill(m_dataSet, "item_base");
            }
            DB.printDataSetContents(m_dataSet, "test");

            // Search the DataSet for a particular record. If not present then retrieve from DB
            dataRows = m_dataSet.Tables["item_base"].Select("id = 4060");
            if (dataRows.Length == 0)
            {
                query = "SELECT * FROM item_base WHERE id = 4060";
                m_sqlDataAdapter = new MySqlDataAdapter(query, myConnection);
                m_sqlDataAdapter.Fill(m_dataSet, "item_base");
            }
            DB.printDataSetContents(m_dataSet, "test");

            // Attempt to delete
            DataRow m_dataRow = dataRows[0];
            m_dataRow.Delete();
            MySqlCommandBuilder mySqlCommandBuilder = new MySqlCommandBuilder(m_sqlDataAdapter);
            m_sqlDataAdapter.Update(m_dataSet, "item_base");
            m_dataSet.AcceptChanges();
            mySqlCommandBuilder.Dispose();
            DB.printDataSetContents(m_dataSet, "test");

            // Search the DataSet for a particular record. If not present then retrieve from DB
            dataRows = m_dataSet.Tables["item_base"].Select("id = 4060");
            if (dataRows.Length == 0)
            {
                query = "SELECT * FROM item_base WHERE id = 4060";
                m_sqlDataAdapter = new MySqlDataAdapter(query, myConnection);
                m_sqlDataAdapter.Fill(m_dataSet, "item_base");
            }
            DB.printDataSetContents(m_dataSet, "test");
        }

        public void test_queryPerformance()
        {
            String query;
            query = "SELECT id FROM item_base WHERE id = 507"; // Average: 257.96 milliseconds
            query = "SELECT *  FROM item_base WHERE id = 507"; // Average: 481.04 milliseconds
            List<String> parameters = new List<string>();
            List<String> values = new List<string>();
            System.DateTime start;
            TimeSpan duration;
            List<TimeSpan> durationList = new List<TimeSpan>();
            int records = 0;
            for (int test = 0; test < 10; test++)
            {
                start = DateTime.Now;
                DataTable dataTable = DB.Instance.executeQuery(query,
                                                               parameters.ToArray(),
                                                               values.ToArray());
                duration = DateTime.Now - start;
                durationList.Add(duration);
                records = dataTable.Rows.Count;
System.Console.WriteLine("******** {0} records.", records);
            }
            double totalTimeSpan = 0;
            for (int test = 0; test < durationList.Count; test++)
            {
                System.Console.WriteLine("{0} milliseconds.", durationList[test].TotalMilliseconds);
                if (test > 0)
                {
                    totalTimeSpan += durationList[test].TotalMilliseconds;
                }
            }
            System.Console.WriteLine("Average: {0} milliseconds for {1} records.", totalTimeSpan / (double)durationList.Count - 1, records);
        }

        public void test_parameterQuery()
        {
            dataSet = new DataSet();
            String query = "SELECT * FROM item_base WHERE id = ?id";
            DataTable dataTable = new DataTable();
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, conn);
            mySqlDataAdapter.SelectCommand.Parameters.Add(new MySqlParameter("?id", 507));
            mySqlDataAdapter.Fill(dataTable);
            Console.WriteLine("==> " + dataTable.Rows.Count);
            if (dataTable.Rows.Count != 0)
            {
                foreach (DataRow row in dataTable.Rows)
                {
                    Console.WriteLine(row["id"] + ": " + row["name"]);
                }
            }

        }

        public void test_old()
        {
            MySqlCommandBuilder cmdBldr;

            dataSet = new DataSet();
            String querySelectAllManufacturer = "SELECT * FROM item_manufacturer_base";

            sqlDataAdapter = new MySqlDataAdapter(querySelectAllManufacturer, conn);

		    // Fill-in the insert, update, and delete commands
            cmdBldr = new MySqlCommandBuilder(sqlDataAdapter);
            if (false)
            {
                Console.WriteLine("Delete: " + cmdBldr.GetDeleteCommand().CommandText);
                Console.WriteLine("Insert: " + cmdBldr.GetInsertCommand().CommandText);
                Console.WriteLine("Update: " + cmdBldr.GetUpdateCommand().CommandText);
            }

            // Copy the data into a local table (of the same name)
		    sqlDataAdapter.Fill(dataSet, "item_manufacturer_base");

            String querySelectAllItem = "SELECT * FROM item_base LIMIT 10";
            sqlDataAdapter = new MySqlDataAdapter(querySelectAllItem, conn);
            cmdBldr = new MySqlCommandBuilder(sqlDataAdapter);
            if (false)
            {
                Console.WriteLine("Delete: " + cmdBldr.GetDeleteCommand().CommandText);
                Console.WriteLine("Insert: " + cmdBldr.GetInsertCommand().CommandText);
                Console.WriteLine("Update: " + cmdBldr.GetUpdateCommand().CommandText);
            }
            sqlDataAdapter.Fill(dataSet, "item_base");

            Console.WriteLine("Tables.Count: " + dataSet.Tables.Count);
            if (false)
            {
                foreach (DataTable dataTable in dataSet.Tables)
                {
                    Console.WriteLine("Table: " + dataTable.TableName);
                }
            }
            //Console.WriteLine("Rows.Count: " + dataSet.Tables[0].Rows.Count);
            //Console.WriteLine("[4][1]: " + dataSet.Tables[0].Rows[4][1].ToString());
            //dataSet.Tables[0].Rows[4][1] = "Sparta";
            //sqlDataAdapter.Update(dataSet, "item_manufacturer_base");


            DataRelation dataRelation = new DataRelation("ManufacturerOfItems",
                                                         dataSet.Tables["item_manufacturer_base"].Columns["id"],
                                                         dataSet.Tables["item_base"].Columns["manufacturer"]
                                                         //, dataSet.Tables["item_manufacturer_base"].Columns["id"]
                                                         );
            dataSet.Relations.Add(dataRelation);
            if (false)
            {
                foreach (DataRow r in dataSet.Tables["item_base"].Rows)
                {
                    Console.WriteLine("Item: " + r["id"] + ". " + r["name"]);
                    DataRow[] childrows = r.GetChildRows("ManufacturerOfItem");
                    foreach (DataRow cr in childrows)
                    {
                        Console.WriteLine("\tManufacturer: " + cr["name"]);
                    }
                }
            }
            if (true)
            {
                // Display the items manufactured by a manufacturer
                foreach (DataRow r in dataSet.Tables["item_manufacturer_base"].Rows)
                {
                    Console.WriteLine("Manufacturer: " + r["id"] + ". " + r["name"]);
                    DataRow[] childrows = r.GetChildRows("ManufacturerOfItems");
                    foreach (DataRow cr in childrows)
                    {
                        Console.WriteLine("\tItem: " + cr["id"] + ". " + cr["name"]);
                    }
                }
            }
        }

    }
*/
}
