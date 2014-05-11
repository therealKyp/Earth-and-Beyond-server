#include "ItemBaseImporter.h"
#include "stdio.h"

#define ITEM_BASE_MAX 6000

int main()
{
    char m_SQL_Dase_Host[128];
    char m_SQL_Dase_DB[128];
    char m_SQL_Dase_User[128];
    char m_SQL_Dase_Pass[128];

    char m_SQL_Net7_Host[128];
    char m_SQL_Net7_DB[128];
    char m_SQL_Net7_User[128];
    char m_SQL_Net7_Pass[128];

    int ClearNet7 = 0;

	FILE *f = fopen("Config.cfg", "r");
    if (f)
    {
		char buffer[1024];
		char *cmd;
		char *data;

		while (!feof(f))
		{
			if (fgets(buffer, sizeof(buffer), f))
			{
				cmd = strtok(buffer, "=");
				data = strtok(NULL, "\n\r");

				if (strcmp(cmd, "mysql_dase_user")==0)
				{
					strcpy(m_SQL_Dase_User, data);
				}
				else if (strcmp(cmd, "mysql_dase_pass")==0)
				{
					strcpy(m_SQL_Dase_Pass, data);
				}
				else if (strcmp(cmd, "mysql_dase_host")==0)
				{
					strcpy(m_SQL_Dase_Host, data);
				}
				else if (strcmp(cmd, "mysql_dase_db")==0)
				{
					strcpy(m_SQL_Dase_DB, data);
				}
				else if (strcmp(cmd, "mysql_net7_user")==0)
				{
					strcpy(m_SQL_Net7_User, data);
				}
				else if (strcmp(cmd, "mysql_net7_pass")==0)
				{
					strcpy(m_SQL_Net7_Pass, data);
				}
				else if (strcmp(cmd, "mysql_net7_host")==0)
				{
					strcpy(m_SQL_Net7_Host, data);
				}
				else if (strcmp(cmd, "mysql_net7_db")==0)
				{
					strcpy(m_SQL_Net7_DB, data);
				}
				else if (strcmp(cmd, "clear_net7_table")==0)
				{
					ClearNet7 = atoi(data);
				}
            }
        }

        fclose(f);
    }

    //Start importing data

    ItemBaseImporter * m_Import = new ItemBaseImporter(ITEM_BASE_MAX);

    //m_Import->ImportXML("C:/Net7/database/ItemBase.xml");

    printf("Starting Dase Conversion\n");
    m_Import->ConvertDase(m_SQL_Dase_DB, m_SQL_Dase_Host, m_SQL_Dase_User, m_SQL_Dase_Pass, m_SQL_Net7_DB, 
        m_SQL_Net7_Host, m_SQL_Net7_User, m_SQL_Net7_Pass, ClearNet7);

    printf("Parsing Complete. %d Items found\n\n", m_Import->ItemCount());

    //m_Import->Normalize();
    //m_Import->ExportXML("C:/Net7/database/ItemBase_NEW.xml");

    printf("Operations finished\n");

    delete m_Import;

    _sleep(20000);

    return 0;
}