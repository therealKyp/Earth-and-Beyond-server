using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;



namespace EnBIniParser
{
    class Program
    {
        static void Main(string[] args)
        {
            SQLData.Host = "net-7.org";
            SQLData.Port = 3307;
            SQLData.User = "Imp";
            SQLData.Pass = "vvx2q7j84";

            new SkillParser();

            //new BuffParser(157, 159);

            //new BaseAssetParser();
            //new BaseAsset();

            //new EffectsParser(0, 1191);
            //new EffectsParser(10000, 10285);
            //new EffectsParser(20000, 20420);
            //new EffectsParser(30000, 30010);

            //new SkillParser();
            //new SkillAbilityParser();

            Console.ReadLine();
        }
    }
}
