using GOSTool.DataHandler;

namespace GOSTool
{
    public class ValueHandler
    {
        public delegate void ConverterDelegate<T1, T>(DataVariable<T1> input, out DataVariable<T> output);

        public delegate void ConverterDelegateTwoParameters<T2, T1, T>(DataVariable<T2> input1, DataVariable<T1> input2, out DataVariable<T> output);

        public static void SetBinding<T, T1>(object target, string property, DataVariable<T> source, ConverterDelegate<T, T1> converter = null)
        {
            source.OnValueChanged += (object sender, T _newValue) =>
            {
                if (converter != null)
                {
                    DataVariable<T1> val = null;
                    converter?.Invoke(source, out val);
                    target.GetType().GetProperty(property).SetValue(target, val.Value);
                }
                else
                {
                    target.GetType().GetProperty(property).SetValue(target, source.Value);
                }
            };
        }

        public static void SetBinding<T, T1, T2>(object target, string property, DataVariable<T> source1, DataVariable<T1> source2, ConverterDelegateTwoParameters<T, T1, T2> converter = null)
        {
            source1.OnValueChanged += (object sender, T _newValue) =>
            {
                if (converter != null)
                {
                    DataVariable<T2> val = null;
                    converter?.Invoke(source1, source2, out val);
                    target.GetType().GetProperty(property).SetValue(target, val.Value);
                }
                else
                {
                    target.GetType().GetProperty(property).SetValue(target, source1.Value);
                }
            };

            source2.OnValueChanged += (object sender, T1 _newValue) =>
            {
                if (converter != null)
                {
                    DataVariable<T2> val = null;
                    converter?.Invoke(source1, source2, out val);
                    target.GetType().GetProperty(property).SetValue(target, val.Value);
                }
                else
                {
                    target.GetType().GetProperty(property).SetValue(target, source2.Value);
                }
            };
        }
    }
}
