using System;
using System.Collections.Generic;
using System.Text;
using CommonTools.Database;

namespace Net7_Tools.Database
{
    /// <summary>Validate a data field</summary>
    /// <remarks>
    ///     Some validations cannt be added within TableIO's addValidation()
    ///     such as those that need to query the data of another field or
    ///     those that need to access a "current" state, such as whether the
    ///     field is enabled or not or whether the record is in add or edit
    ///     mode.  In those cases the data validation needs to be performed
    ///     within IRecordManager's validField().
    /// </remarks>
    public class DataValidation
    {
        public virtual Result validate(String value)
        {
            return new Result();
        }

        /// <summary>Result of validations</summary>
        /// <returns>If there's a validation error then the explaination is contained within the message</returns>
        /// <remarks>
        ///     When m_message contains null then there isn't a validation error.  It's only
        ///     when m_message contains a message that there is an error.
        /// </remarks>
        public class Result
        {
            private String m_message;
            private String m_controlName;

            public Result()
            {
                m_message = null;
            }

            public Result(Result result)
            {
                m_message = result.m_message;
            }

            public Result(String message)
            {
                m_message = message;
            }

            public Boolean isValid()
            {
                return m_message == null;
            }

            public String getMessage()
            {
                return isValid() ? "" : m_message;
            }

            public void setMessage(String message)
            {
                m_message = message;
            }

            public void setControlName(String controlName)
            {
                m_controlName = controlName;
            }

            public String getControlName()
            {
                return m_controlName;
            }

            public void showMessage()
            {
                System.Windows.Forms.MessageBox.Show(getMessage(), "Error in '" + m_controlName + "'");
            }
        }

        public static Result isNonEmpty(CodeValue codeValue)
        {
            if (codeValue == null || codeValue.value == null)
            {
                return new DataValidation.Result("The value is empty");
            }
            else
            {
                return new DataValidation.Result();
            }
        }

        public static Result isNonEmpty(String value)
        {
            if (value == null || value.Length == 0)
            {
                return new DataValidation.Result("The value is empty");
            }
            else
            {
                return new DataValidation.Result();
            }
        }

        public static Result isNumeric(String value)
        {
            Decimal number;
            DataValidation.Result result = isNonEmpty(value);
            if (result.isValid()
                && !Decimal.TryParse(value, out number))
            {
                result.setMessage("The value '" + value + "' is not a number");
            }
            return result;
        }

        public static Result isInteger(String value)
        {
            Int32 number;
            DataValidation.Result result = isNonEmpty(value);
            if (result.isValid()
                && !Int32.TryParse(value, out number))
            {
                result.setMessage("The value '" + value + "' is not an integer");
            }
            return result;
        }

        public static Result isNumericEqual(String value, Decimal targetValue)
        {
            Decimal number = 0;
            DataValidation.Result result = isNumeric(value);
            if (result.isValid())
            {
                Decimal.TryParse(value, out number);
                if (number != targetValue)
                {
                    result.setMessage("The value is different than " + targetValue.ToString());
                }
            }
            return result;
        }

        public static Result isNumericDifferent(String value, Decimal targetValue)
        {
            Decimal number = 0;
            DataValidation.Result result = isNumeric(value);
            if (result.isValid())
            {
                Decimal.TryParse(value, out number);
                if (number == targetValue)
                {
                    result.setMessage("The value is equal to " + targetValue.ToString());
                }
            }
            return result;
        }

        public static Result isNumericInsideRange(String value, Decimal minimum, Decimal maximum)
        {
            Decimal number = 0;
            DataValidation.Result result = isNumeric(value);
            if (result.isValid())
            {
                Decimal.TryParse(value, out number);
                if (number < minimum)
                {
                    result.setMessage("The value '" + value + "' is lesser than the minimum of " + minimum.ToString());
                }
                else if (number > maximum)
                {
                    result.setMessage("The value '" + value + "' is lesser than the minimum of " + minimum.ToString());
                }
            }
            return result;
        }

        public static Result isNumericOutsideRange(String value, Decimal lowerBound, Decimal upperBound)
        {
            Decimal number = 0;
            DataValidation.Result result = isNumeric(value);
            if (result.isValid())
            {
                Decimal.TryParse(value, out number);
                if (number >= lowerBound && number <= upperBound)
                {
                    result.setMessage("The value '" + value + "' is within the bounds of " + lowerBound.ToString() + " and " + upperBound.ToString());
                }
            }
            return result;
        }

        public static Result isNumericGreater(String value, Decimal minimum)
        {
            Decimal number = 0;
            DataValidation.Result result = isNumeric(value);
            if (result.isValid())
            {
                Decimal.TryParse(value, out number);
                if (number <= minimum)
                {
                    result.setMessage("The value is not greater than the minimum of " + minimum.ToString());
                }
            }
            return result;
        }

        public static Result isNumericGreaterEqual(String value, Decimal minimum)
        {
            Decimal number = 0;
            DataValidation.Result result = isNumeric(value);
            if (result.isValid())
            {
                Decimal.TryParse(value, out number);
                if (number < minimum)
                {
                    result.setMessage("The value is not greater or equal to the minimum of " + minimum.ToString());
                }
            }
            return result;
        }

        public static Result isNumericLesser(String value, Decimal maximum)
        {
            Decimal number = 0;
            DataValidation.Result result = isNumeric(value);
            if (result.isValid())
            {
                Decimal.TryParse(value, out number);
                if (number >= maximum)
                {
                    result.setMessage("The value is not lesser than the maximum of " + maximum.ToString());
                }
            }
            return result;
        }

        public static Result isNumericLesserEqual(String value, Decimal maximum)
        {
            Decimal number = 0;
            DataValidation.Result result = isNumeric(value);
            if (result.isValid())
            {
                Decimal.TryParse(value, out number);
                if (number > maximum)
                {
                    result.setMessage("The value is not lesser or equal to the maximum of " + maximum.ToString());
                }
            }
            return result;
        }

        public static Result isInTable(String value, String tableName, String query)
        {
            System.Data.DataTable dataTable = DB.Instance.executeQuery(query, null, null);
            if (dataTable.Rows.Count != 0)
            {
                // It's present at least once
                return new Result();
            }
            else
            {
                return new Result("The value '" + value + "' does not exist within the '" + tableName + "' table");
            }
        }

        public static Result isNotInTable(String value, String tableName, String query)
        {
            System.Data.DataTable dataTable = DB.Instance.executeQuery(query, null, null);
            if (dataTable.Rows.Count == 0)
            {
                // It's not present
                return new Result();
            }
            else
            {
                return new Result("The value '" + value + "' exists within the '" + tableName + "' table");
            }
        }
    }

    class ValidateNonEmpty : DataValidation
    {
        public override Result validate(String value)
        {
            return isNonEmpty(value);
        }
    }

    class ValidateNumeric : DataValidation
    {
        public override Result validate(String value)
        {
            return isNumeric(value);
        }
    }

    class ValidateInteger : DataValidation
    {
        public override Result validate(String value)
        {
            return isInteger(value);
        }
    }

    class ValidateNumericEqual : DataValidation
    {
        Decimal m_targetValue;

        public ValidateNumericEqual(Decimal targetValue)
        {
            m_targetValue = targetValue;
        }

        public override Result validate(String value)
        {
            return isNumericEqual(value, m_targetValue);
        }
    }

    class ValidateNumericDifferent : DataValidation
    {
        Decimal m_targetValue;

        public ValidateNumericDifferent(Decimal targetValue)
        {
            m_targetValue = targetValue;
        }

        public override Result validate(String value)
        {
            return isNumericDifferent(value, m_targetValue);
        }
    }

    class ValidateNumericInsideRange : DataValidation
    {
        Decimal m_minimum;
        Decimal m_maximum;

        public ValidateNumericInsideRange(Decimal minimum, Decimal maximum)
        {
            m_minimum = minimum;
            m_maximum = maximum;
        }

        public override Result validate(String value)
        {
            return isNumericInsideRange(value, m_minimum, m_maximum);
        }
    }

    class ValidateNumericOutsideRange : DataValidation
    {
        Decimal m_lowerBound;
        Decimal m_upperBound;

        public ValidateNumericOutsideRange(Decimal lowerBound, Decimal upperBound)
        {
            m_lowerBound = lowerBound;
            m_upperBound = upperBound;
        }

        public override Result validate(String value)
        {
            return isNumericOutsideRange(value, m_lowerBound, m_upperBound);
        }
    }

    class ValidateNumericGreater : DataValidation
    {
        Decimal m_minimum;

        public ValidateNumericGreater(Decimal minimum)
        {
            m_minimum = minimum;
        }

        public override Result validate(String value)
        {
            return isNumericGreater(value, m_minimum);
        }
    }

    class ValidateNumericGreaterEqual : DataValidation
    {
        Decimal m_minimum;

        public ValidateNumericGreaterEqual(Decimal minimum)
        {
            m_minimum = minimum;
        }

        public override Result validate(String value)
        {
            return isNumericGreaterEqual(value, m_minimum);
        }
    }

    class ValidateNumericLesser : DataValidation
    {
        Decimal m_maximum;

        public ValidateNumericLesser(Decimal maximum)
        {
            m_maximum = maximum;
        }

        public override Result validate(String value)
        {
            return isNumericLesser(value, m_maximum);
        }
    }

    class ValidateNumericLesserEqual : DataValidation
    {
        Decimal m_maximum;

        public ValidateNumericLesserEqual(Decimal maximum)
        {
            m_maximum = maximum;
        }

        public override Result validate(String value)
        {
            return isNumericLesserEqual(value, m_maximum);
        }
    }

    class ValidateInTable : DataValidation
    {
        String m_table;
        String m_query;

        public ValidateInTable(String table, String query)
        {
            m_table = table;
            m_query = query;
        }

        public override Result validate(String value)
        {
            return isInTable(value, m_table, m_query);
        }
    }

    class ValidateNotInTable : DataValidation
    {
        String m_table;
        String m_query;

        public ValidateNotInTable(String table, String query)
        {
            m_table = table;
            m_query = query;
        }

        public override Result validate(String value)
        {
            return isNotInTable(value, m_table, m_query);
        }
    }

    class ValidationConditional : DataValidation
    {
        Boolean m_condition;
        DataValidation m_dataValidationWhenTrue;
        DataValidation m_dataValidationWhenFalse;

        public ValidationConditional(Boolean condition, DataValidation whenConditionTrue, DataValidation whenConditionFalse)
        {
            m_condition = condition;
            m_dataValidationWhenTrue = whenConditionTrue;
            m_dataValidationWhenFalse = whenConditionFalse;
        }

        public override Result validate(String value)
        {
            if (m_condition)
            {
                return m_dataValidationWhenTrue.validate(value);
            }
            else
            {
                return m_dataValidationWhenFalse.validate(value);
            }
        }
    }
}
