using System;
using System.Collections.Generic;
using System.Text;

namespace Net7_Tools.Database
{
    class ItemType
    {
    }
}


#region --== Read a text file and generate a unique list ==--
/* The value of cCategory must be changed to reflect the appropriate
 * category: AmmunitionLauncherType, BeamType, MissileLauncherAmmoType,
 * ProjectileLauncherAmmoType.
 * Progress source code:
DEF VAR cLine AS CHAR.
DEF VAR cCategory AS CHAR INIT "AmmunitionLauncherType".

DEFINE TEMP-TABLE ttTypes
  FIELD cCategory AS CHARACTER
  FIELD cType     AS CHARACTER.


INPUT FROM c:\types.txt.
REPEAT:
  cLine = "".
  IMPORT UNFORMATTED cLine.
  cLine = TRIM(cLine).
  IF cLine NE "" 
    AND NOT CAN-FIND(ttTypes
               WHERE ttTypes.cType EQ cLine)
  THEN DO:
    CREATE ttTypes.
    ASSIGN
      ttTypes.cCategory = cCategory
      ttTypes.cType     = cLine.
    RELEASE ttTypes.
  END.
END.
INPUT CLOSE.

OUTPUT TO c:\TypesClean.txt APPEND.
FOR EACH ttTypes:
  EXPORT ttTypes.
END.
OUTPUT CLOSE.
 */
#endregion

#region --== Read a unique list and generate an XML or an SQL file ==--
/*
DEFINE TEMP-TABLE ttTypes
  FIELD cCategory AS CHARACTER
  FIELD cType     AS CHARACTER
  FIELD iId       AS INTEGER.

INPUT FROM c:\TypesClean.txt.
REPEAT:
  CREATE ttTypes.
  IMPORT ttTypes.
END.
INPUT CLOSE.


DEFINE VARIABLE iId             AS INTEGER.
DEFINE VARIABLE cId             AS CHARACTER.

DEFINE VARIABLE iSubCategory    AS INTEGER.
DEFINE VARIABLE cSubCategory AS CHARACTER EXTENT 4 INIT ["100", "102", "101", "103"].

DEFINE VARIABLE cBeams AS CHARACTER EXTENT 3 INIT ["EMP", "Energy", "Plasma"].
DEFINE VARIABLE iBeams AS INTEGER.
DEFINE STREAM sOut.

OUTPUT STREAM sOut TO VALUE("c:\Types.txt").
ASSIGN iSubCategory = 1.
DO iBeams = 1 TO EXTENT(cBeams):
  ASSIGN
    iId         = iId + 1
    ttTypes.iID = iID.
  RUN OutputData(cSubCategory[iSubCategory], STRING(iID), cBeams[iBeams]).
END.


DEFINE VARIABLE iType AS INTEGER.
DEFINE VARIABLE cType AS CHARACTER EXTENT 2 INIT ["MissileLauncherAmmoType", "ProjectileLauncherAmmoType"].
DO iType = 1 TO EXTENT(cType):
  ASSIGN iSubCategory = iSubCategory + 1.
  FOR EACH ttTypes
    WHERE ttTypes.cCategory EQ cType[iType]
    BREAK BY ttTypes.cType:
    IF ttTypes.cType NE "" AND FIRST-OF(ttTypes.cType)
    THEN DO:
      ASSIGN
        iId         = iId + 1
        ttTypes.iID = iID.
      RUN OutputData(cSubCategory[iSubCategory], STRING(iID), ttTypes.cType).
    END.
  END.
END.



IF FALSE
THEN DO:
  DEFINE BUFFER b_ttTypes FOR ttTypes.
  ASSIGN iSubCategory = iSubCategory + 1.
  FOR EACH ttTypes
    WHERE ttTypes.cCategory EQ "AmmunitionLauncherType"
    BREAK BY ttTypes.cType:
    IF ttTypes.cType NE "" AND FIRST-OF(ttTypes.cType)
    THEN DO:
      FIND b_ttTypes
        WHERE b_ttTypes.cType EQ ttTypes.cType
        AND   b_ttTypes.iID   NE 0
        NO-LOCK NO-ERROR.
      ASSIGN
        iId = IF AVAILABLE b_ttTypes
              THEN b_ttTypes.iID
              ELSE 0.
      RUN OutputData(cSubCategory[iSubCategory], STRING(iID), ttTypes.cType).
    END.
  END.
END.

OUTPUT STREAM sOut CLOSE.



PROCEDURE OutputData:
  DEFINE INPUT PARAMETER pcSubCategory AS CHARACTER.
  DEFINE INPUT PARAMETER pcID          AS CHARACTER.
  DEFINE INPUT PARAMETER pcName        AS CHARACTER.

  RUN OutputSqlData(pcSubCategory, pcID, pcName).
END.


PROCEDURE OutputXmlData:
  DEFINE INPUT PARAMETER pcSubCategory AS CHARACTER.
  DEFINE INPUT PARAMETER pcID          AS CHARACTER.
  DEFINE INPUT PARAMETER pcName        AS CHARACTER.

  ASSIGN pcID = '"' + pcID + '"'.
  IF LENGTH(pcID) LT 5
  THEN pcID = FILL(" ", 5 - LENGTH(pcID)) + pcID.
  PUT STREAM sOut UNFORMATTED
    '<TYPE subcategory = "' + pcSubCategory + '" id = ' + pcID + ' name = "' + pcName + '"/>' SKIP.
END.


PROCEDURE OutputSqlData:
  DEFINE INPUT PARAMETER pcSubCategory AS CHARACTER.
  DEFINE INPUT PARAMETER pcID          AS CHARACTER.
  DEFINE INPUT PARAMETER pcName        AS CHARACTER.

  IF SEEK(sOut) EQ 0
  THEN DO:
    PUT STREAM sOut UNFORMATTED
      "-- To use this file, load its contents into the mysql database. For example," SKIP
      "-- with the mysql client program, process the file like this, where" SKIP
      "-- file_name is the name of this file:" SKIP
      " " SKIP
      "--     mysql -u root -p mysql < file_name" SKIP
      " " SKIP
      "set names 'utf8';" SKIP
      " " SKIP
      "set storage_engine=InnoDB;" SKIP
      " " SKIP
      "--" SKIP
      "-- The item_type table" SKIP
      "--" SKIP
      "CREATE TABLE IF NOT EXISTS net7_db.item_type ( id INTEGER UNSIGNED NOT NULL, sub_category SMALLINT NOT NULL, name VARCHAR(64) NOT NULL, PRIMARY KEY (id));" SKIP
      " " SKIP
      "delete from net7_db.item_type;" SKIP
      " " SKIP.
  END.


  ASSIGN
    pcSubCategory = REPLACE(pcSubCategory, "'", "''")
    pcID          = REPLACE(pcID, "'", "''")
    pcName        = REPLACE(pcName, "'", "''").
  PUT STREAM sOut UNFORMATTED
    "insert into net7_db.item_type (sub_category, id, name) values ("
    + pcSubCategory
    + ", "
    + pcID
    + ", '"
    pcName
    + "');" SKIP.
END.

*/
#endregion
