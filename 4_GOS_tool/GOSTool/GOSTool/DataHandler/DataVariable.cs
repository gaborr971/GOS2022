namespace GOSTool.DataHandler
{
    public class DataVariable<T>
    {
        public T Value { get; private set; } = default;
        
        public delegate void OnValueChangedDelegate<U>(object sender, U newValue);

        public event OnValueChangedDelegate<T> OnValueChanged;

        public DataVariable()
        {

        }

        public DataVariable(T value)
        {
            SetValue(value);
        }

        public void SetValue(T value)
        {
            Value = value;
            OnValueChanged?.Invoke(this, value);
        }
    }
}
