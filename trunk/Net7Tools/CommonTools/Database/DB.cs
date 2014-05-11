using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MySql.Data.MySqlClient;
using System.Data;
using System.IO;

namespace CommonTools.Database
{
    public class DB : Singleton.Get<DB>
    {
        /// <summary>
        /// The name of the database
        /// </summary>
        public const String DATABASE_NAME = "Net7";

        public const String SELECT = "SELECT ";
        public const String FROM = " FROM ";
        public const String WHERE = " WHERE ";
        public const String EQUALS = " = ";
        public const String ORDER_BY = " ORDER BY ";
        public const String LIMIT = " LIMIT ";
        public const String LIMIT1 = " LIMIT 1";

        private Boolean m_showExecutionTime = false;

        /// <summary>
        /// A MySQL transaction
        /// </summary>
        private MySqlTransaction m_mySqlTransaction;

        /// <summary>
        ///   <para>MySQL query parameters use the ? character rather than @</para>
        /// </summary>
        public const String QueryParameterCharacter = "?";

        /// <summary>
        /// The MySQL connection
        /// </summary>
        private MySqlConnection m_mySqlConnection = null;

        /// <summary>
        /// Constructor
        /// </summary>
        public DB()
        {
        }

        /// <summary>
        /// Open a connection to the database
        /// </summary>
        /// <param name="databaseName"></param>
        /// <returns></returns>
        public MySqlConnection openConnection()
        {
            if (m_mySqlConnection == null)
            {
                m_mySqlConnection = new MySqlConnection(CommonTools.Gui.LoginData.ConnStr(DATABASE_NAME));
            }

            if (m_mySqlConnection.State.ToString().Equals("Closed"))
            {
                m_mySqlConnection.Open();
            }

            return m_mySqlConnection;
        }

        public void closeConnection()
        {
            m_mySqlConnection.Close();
        }

        public void startTransaction()
        {
            // Ensure the connection is opened
            openConnection();
            m_mySqlTransaction = m_mySqlConnection.BeginTransaction();
        }

        public void commitTransaction()
        {
            m_mySqlTransaction.Commit();
            m_mySqlTransaction = null;
        }

        public void rollbackTransaction()
        {
            m_mySqlTransaction.Rollback();
            m_mySqlTransaction = null;
        }

        /// <summary>
        ///   <para>Common procedure to execute a query.</para>
        /// </summary>
        /// <param name="query">The SQL query to execute.</param>
        /// <param name="parameter">The query parameter to fill in</param>
        /// <param name="value">The value of the query parameter to use</param>
        public DataTable executeQuery(String query, String[] parameter, String[] value)
        {
            DataTable dataTable = null;
            MySqlDataAdapter mySqlDataAdapter = null;
            try
            {
                dataTable = new DataTable();
                openConnection();

                mySqlDataAdapter = new MySqlDataAdapter(query, m_mySqlConnection);
                mySqlDataAdapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;

                if (parameter != null && parameter.Length != 0)
                {
                    for (int parameterIndex = 0; parameterIndex < parameter.Length; parameterIndex++)
                    {
                        mySqlDataAdapter.SelectCommand.Parameters.Add(new MySqlParameter(parameter[parameterIndex], value[parameterIndex]));
                    }
                }

                if (m_mySqlTransaction != null)
                {
                    mySqlDataAdapter.SelectCommand.Transaction = m_mySqlTransaction;
                }

                DateTime start = DateTime.Now;
                mySqlDataAdapter.Fill(dataTable); // 156.245 milliseconds.
                if (m_showExecutionTime)
                {
                    TimeSpan timeSpan = DateTime.Now - start;
                    System.Console.WriteLine(query + ": {0} milliseconds, {1} rows.", timeSpan.TotalMilliseconds, dataTable.Rows.Count);
                }
            }
            catch (Exception e)
            {
                String values = "";
                if (value != null)
                {
                    foreach (String val in value)
                    {
                        if (values.Length != 0)
                        {
                            values += ", ";
                        }
                        values += val;
                    }
                }
                System.Windows.Forms.MessageBox.Show(e.Message + "\n\n"
                                                        + e.StackTrace + "\n\n"
                                                        + query
                                                        + "\n" + values + "\n\n"
                                                        + query,
                                                     "Error within DB.executeQuery()");
            }
            finally
            {
                if (m_mySqlConnection != null)
                {
                    // Should close here but since we are keeping the connection open
                    // we won't close it here
                    //m_mySqlConnection.Close();
                }
            }
            return dataTable;
        }

        public int executeCommand(String query, String[] parameter, String[] value)
        {
            int rowsAffected = 0;
            try
            {
                openConnection();

                MySqlCommand mySqlCommand = new MySqlCommand(query, m_mySqlConnection);

                if (parameter != null && parameter.Length != 0)
                {
                    for (int parameterIndex = 0; parameterIndex < parameter.Length; parameterIndex++)
                    {
                        mySqlCommand.Parameters.Add(new MySqlParameter(parameter[parameterIndex], value[parameterIndex]));
                    }
                }

                if (m_mySqlTransaction != null)
                {
                    mySqlCommand.Transaction = m_mySqlTransaction;
                }

                DateTime start = DateTime.Now;
                rowsAffected = mySqlCommand.ExecuteNonQuery();
                if (m_showExecutionTime)
                {
                    TimeSpan timeSpan = DateTime.Now - start;
                    System.Console.WriteLine(query + ": {0} milliseconds, {1} rows.", timeSpan.TotalMilliseconds, rowsAffected);
                }
            }
            catch (Exception e)
            {
                String values = "";
                if (value != null)
                {
                    foreach (String val in value)
                    {
                        if (values.Length != 0)
                        {
                            values += ", ";
                        }
                        values += val;
                    }
                }
                System.Windows.Forms.MessageBox.Show(e.Message + "\n\n"
                                                        + e.StackTrace + "\n\n"
                                                        + query
                                                        + "\n" + values + "\n\n"
                                                        + query,
                                                     "Error within DB.executeCommand()");
            }
            finally
            {
                if (m_mySqlConnection != null)
                {
                    // Should close here but since we are keeping the connection open
                    // we won't close it here
                    //m_mySqlConnection.Close();
                }
            }
            return rowsAffected;
        }

        /// <summary>
        /// Set whether the time to execute an SQL command is displayed to the console.
        /// </summary>
        /// <param name="show">Whether to show the execution time</param>
        public void showExecutionTime(Boolean show)
        {
            m_showExecutionTime = show;
        }

        public String createSelect(Enum[] field, Net7.Tables table, Enum idField, String value, Int32 queryCount)
        {
            String query = "";
            foreach(Enum enumField in field)
            {
                if (query.Length == 0)
                {
                    query = SELECT;
                }
                else
                {
                    query += ",";
                }
                query += ColumnData.GetName(enumField);
            }
            query += FROM
                   + table.ToString()
                   + WHERE
                   + ColumnData.GetName(idField)
                   + " = "
                   + DB.QueryParameterCharacter
                   + idField.ToString() + queryCount.ToString()
                   + ";";
            //System.Windows.Forms.MessageBox.Show(query, value);
            return query;
        }

        public DataTable select(Enum[] field, Net7.Tables table, Enum idField, String value)
        {
            String query = createSelect(field, table, idField, value, 0);
            return DB.Instance.executeQuery(query, new string[] { idField.ToString() + "0" }, new string[] { value });
        }

        public String getVersion(String toolName)
        {
            String toolParameter = "ToolName";
            String query = "SELECT "
                         + ColumnData.GetName(Net7.Table_versions._Version)
                         + " FROM "
                         + Net7.Tables.versions.ToString()
                         + " WHERE "
                         + ColumnData.GetName(Net7.Table_versions._EName)
                         + " = "
                         + DB.QueryParameterCharacter + toolParameter;

            DataTable dataTable = DB.Instance.executeQuery(query, new String[] { toolParameter }, new String[] { toolName });
            return dataTable.Rows.Count == 0
                 ? ""
                 : ColumnData.GetString(dataTable.Rows[0], Net7.Table_versions._Version);
        }

        public void setVersion(String toolName, String version)
        {
            String toolNameParameter = "ToolName";
            String toolVersionParameter = "ToolVersion";
            String query = "UPDATE "
                         + Net7.Tables.versions
                         + " SET "
                         + ColumnData.GetName(Net7.Table_versions._Version)
                         + " = " + DB.QueryParameterCharacter + toolVersionParameter
                         + " WHERE "
                         + ColumnData.GetName(Net7.Table_versions._EName)
                         + " = "
                         + DB.QueryParameterCharacter + toolNameParameter;

            int rowsAffected = DB.Instance.executeCommand(query, new String[] { toolVersionParameter, toolNameParameter }, new String[] { version, toolName });
            if (rowsAffected == 0)
            {
                // INSERT INTO Persons (P_Id, LastName, FirstName) VALUES (5, 'Tjessem', 'Jakob')
                query = "INSERT INTO "
                      + Net7.Tables.versions
                      + " ("
                      + ColumnData.GetName(Net7.Table_versions._EName) + ","
                      + ColumnData.GetName(Net7.Table_versions._Version)
                      + ") VALUES ("
                      + DB.QueryParameterCharacter + toolNameParameter + ","
                      + DB.QueryParameterCharacter + toolVersionParameter
                      + ")";
                rowsAffected = DB.Instance.executeCommand(query, new String[] { toolVersionParameter, toolNameParameter }, new String[] { version, toolName });
                if (rowsAffected == 0)
                {
                    System.Windows.Forms.MessageBox.Show("Unable to update/create the version information into the "
                                                       + Net7.Tables.versions
                                                       + " table"); 
                }
            }
        }

        /// <summary>
        /// Import the contents of a file into a database table
        /// </summary>
        /// <param name="table">The name of the table</param>
        /// <param name="valuesFile">The file name to import.  The contents of this file
        /// are expected to contain one or multiple rows, where each field of the table
        /// is present.</param>
        public void importValues(Net7.Tables table, String valuesFile)
        {
            String query;
            String row;
            StreamReader tr = new StreamReader(valuesFile);
            while (!tr.EndOfStream)
            {
                row = tr.ReadLine();
                query = "INSERT INTO "
                      + table
                      + " VALUES ("
                      + row
                      + ")";
                int rowsAffected = DB.Instance.executeCommand(query, null, null);
                if (rowsAffected == 0)
                {
                    System.Windows.Forms.MessageBox.Show("Error inserting the following row:\n" + row);
                }
            }
            tr.Close();
        }

        /// <summary>
        ///   <para>Convert a database structure into various enumerations.</para>
        /// </summary>
        /// <param name="databaseName">The database name for which to generate the code.</param>
        /// <remarks>This method overwrites the contents of the &lt;databaseName&gt;.cs file.
        ///          The goal of this approach is to easily handle schema changes without having
        ///          to hunt through code in order to locate strings that now point to obsolete
        ///          names.  Instead those will now generate compile-time errors.</remarks>
        public void makeDatabaseVariables()
        {
            String query;
            String tableName;
            String tableEnum;
            String columnAlignedPosition;

            DataTable dataTable;
            dataTable = executeQuery("SELECT DISTINCT table_name "
                                   + "FROM information_schema.columns "
                                   + "WHERE table_schema = '"
                                   + DATABASE_NAME
                                   + "'", null, null);

            System.IO.FileInfo fileInfo = new System.IO.FileInfo("..\\..\\..\\..\\CommonTools\\Database\\" + DATABASE_NAME + ".cs");
            System.IO.StreamWriter streamWriter = fileInfo.CreateText();

            streamWriter.WriteLine("// This file was automatically generated by Database.makeDatabaseVariables() on "
                                  + DateTime.Now.Year.ToString() + "/"
                                  + DateTime.Now.Month.ToString() + "/"
                                  + DateTime.Now.Day.ToString() + " "
                                  + DateTime.Now.Hour.ToString() + ":"
                                  + DateTime.Now.Minute.ToString() + ":"
                                  + DateTime.Now.Second.ToString());

            streamWriter.WriteLine("namespace CommonTools.Database");
            streamWriter.WriteLine("{");
            streamWriter.WriteLine("    public static class " + DATABASE_NAME);
            streamWriter.WriteLine("    {");

            tableEnum = "        public enum Tables { ";
            columnAlignedPosition = new String(' ', tableEnum.Length);
            streamWriter.Write(tableEnum);
            DataRow dataRow;
            for (int rowIndex = 0; rowIndex < dataTable.Rows.Count; ++rowIndex)
            {
                dataRow = dataTable.Rows[rowIndex];
                tableName = dataRow["table_name"].ToString();
                if (rowIndex != dataTable.Rows.Count - 1)
                {
                    tableName += ", ";
                }
                streamWriter.Write(tableName);
                if (rowIndex != 0 && (rowIndex % 5) == 0)
                {
                    streamWriter.WriteLine("");
                    streamWriter.Write(columnAlignedPosition);
                }
            }
            streamWriter.WriteLine(" };");
            streamWriter.WriteLine("");

            query = "SELECT table_name, column_name, data_type "
                    + "FROM information_schema.columns "
                    + "WHERE table_schema = '"
                    + DATABASE_NAME
                    + "' "
                    + "ORDER BY table_name, ordinal_position";
            makeDatabaseEnum(streamWriter, query, columnAlignedPosition, false, true);

            query = "SELECT "
                  + "'item_type' as 'table_name', "
                  + "name as 'column_name', "
                  + "id as 'data_type' "
                  + "FROM "
                  + Net7.Tables.item_type.ToString()
                  + " ORDER BY "
                  + ColumnData.GetName(Net7.Table_item_type._id);
            makeDatabaseEnum(streamWriter, query, columnAlignedPosition, true, true);

            streamWriter.WriteLine("    }");
            streamWriter.WriteLine("}");
            streamWriter.Close();
        }

        public void makeDatabaseEnum(System.IO.StreamWriter streamWriter,
                                            String query,
                                            String columnAlignedPosition,
                                            Boolean tableContent,
                                            Boolean forCSharp)
        {
            String tableName;
            String columnName;
            String columnEnum;
            String dataType;
            String tableEnum;
            String enumPrefix = tableContent ? "Enum_" : "Table_";
            DataTable dataTable;
            DataRow dataRow;

            dataTable = executeQuery(query, null, null);

            String previousTableName = null;
            for (int rowIndex = 0; rowIndex < dataTable.Rows.Count; ++rowIndex)
            {
                dataRow = dataTable.Rows[rowIndex];
                tableName = dataRow["table_name"].ToString();
                columnName = dataRow["column_name"].ToString();
                dataType = dataRow["data_type"].ToString();

                columnEnum = columnName;
                columnEnum = "_" + columnEnum; // Ensures a valid enum name
                columnEnum = columnEnum.Replace(" ", "_"); // Not sure how else to handle a space in the name
                if (tableContent)
                {
                    // Add the enum value
                    columnEnum += " = " + dataType;
                }
                else
                {
                    // Add the ColName property
                    columnEnum = "[ColName(\"" + columnName + "\")] "
                               + "[DataType(\"" + dataType + "\")] "
                               + columnEnum;
                }

                if ((previousTableName != null
                     && previousTableName.Equals(tableName))
                    || rowIndex == dataTable.Rows.Count - 1)
                {
                    streamWriter.WriteLine(",");
                    streamWriter.Write(columnAlignedPosition + columnEnum);
                }

                if (previousTableName != null
                    && (rowIndex == dataTable.Rows.Count - 1 || !previousTableName.Equals(tableName)))
                {
                    // End of a table
                    streamWriter.WriteLine(" };");
                    streamWriter.WriteLine("");
                    previousTableName = null;
                }

                if (previousTableName == null
                    && rowIndex != dataTable.Rows.Count - 1)
                {
                    // Start of a new table
                    previousTableName = tableName;
                    tableEnum = "        "
                                + (forCSharp ? "public " : "")
                                + "enum "
                                + enumPrefix
                                + tableName
                                + " { ";
                    columnAlignedPosition = new String(' ', tableEnum.Length);
                    streamWriter.Write(tableEnum + columnEnum);
                }
            }
        }

        public static void printDataSetContents(DataSet dataSet, String title)
        {
            /*
            System.Console.WriteLine("\n" + title + ": " + dataSet.Tables.Count + " tables in the dataset\n" + new String('=', 20));
            foreach (DataTable dataTable in dataSet.Tables)
            {
                System.Console.WriteLine(dataTable.TableName + ": " + dataTable.Rows.Count.ToString() + " rows");
            }
            System.Console.WriteLine("");
            */


            int maxRows;
            System.Console.WriteLine("\n" + title + ": " + dataSet.Tables.Count + " tables in the dataset\n" + new String('=', 20));
            foreach (DataTable dataTable in dataSet.Tables)
            {
                System.Console.WriteLine(dataTable.TableName + ": " + dataTable.Rows.Count.ToString() + " rows");
                System.Console.WriteLine(new String('-', dataTable.TableName.Length));
                maxRows = 10;
                foreach (DataRow dataRow in dataTable.Rows)
                {
                    if (--maxRows == 0)
                    {
                        break;
                    }
                    System.Console.WriteLine(dataRow[0].ToString() + "\t" + dataRow[1].ToString());
                }
                System.Console.WriteLine("");
            }

        }

    }
}
