using System;

namespace CommonTools
{
    public static class Tools
    {
        // TODO: Have ItemEditor's Items.cs reuse this code
        public static String[] SortEnumByName(System.Type enumeration)
        {
            String[] enumNames = Enum.GetNames(enumeration);
            Array.Sort(enumNames);
            return enumNames;
        }

        /// <summary>
        /// Convert a String into an enum value
        /// Inspired from http://kirillosenkov.blogspot.com/2007/09/making-c-enums-more-usable-parse-method.html
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="conditionValue"></param>
        /// <returns></returns>
        public static T ParseEnum<T>(String value)
        {
            if (String.IsNullOrEmpty(value))
            {
                throw new ArgumentNullException("Can't parse an empty string");
            }

            Type enumType = typeof(T);
            if (!enumType.IsEnum)
            {
                throw new InvalidOperationException("This type is not an enumeration");
            }

            T conditionType;
            try
            {
                conditionType = (T)Enum.Parse(typeof(T), value);
            }
            catch (System.ArgumentException)
            {
                // Unable to parse so return the first value of the enum
                // This assumes that the first value is always a "None"
                conditionType = (T)Enum.GetValues(enumType).GetValue(0);
            }
            return conditionType;
        }

        public static String GetEnumString<T>(Object value)
        {
            T enumValue = ParseEnum<T>(value.ToString());
            //((Int32)enumValue).ToString();
            return enumValue.ToString();
        }

        public static ConditionType GetConditionType(String conditionValue)
        {
            ConditionType conditionType;
            try
            {
                conditionType = (ConditionType)Enum.Parse(typeof(ConditionType), conditionValue);
            }
            catch (System.ArgumentException)
            {
                conditionType = ConditionType._None;
            }
            return conditionType;
        }
    }

    public enum ConditionType
    {
        _None = 0,
        Overall_Level = 1,
        Combat_Level,
        Explore_Level,
        Trade_Level,
        Race,
        Profession,
        Hull_Level,
        Faction_Required,
        Item_Required
    }

    public enum TalkTreeFlag
    {
        _None = 0,          // 0. Default, unassigned value
        Trade = 1,			// 1. Gives access to trade
        Postpone_Mission,	// 2. Allows to drop the mission and continue later
        Drop_Mission,		// 3. Won't be able to do the mission again
        More,				// 4. 'more' dialogue box
        Mission_Goto_Stage,	// 5. Need to branch to another mission node (set as Destination="x").
        Mission_Complete    // 6. Done
    }

}