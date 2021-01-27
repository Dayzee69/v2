using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Rib
{
    /// <summary>
    /// Логика взаимодействия для OrderWindow.xaml
    /// </summary>
    
    public partial class OrderWindow : Window
    {
        string month = "";
        MainWindow mainWindow;
        public OrderWindow(MainWindow window)
        {
            mainWindow = window;
            InitializeComponent();
        }

        
        public void bOpen_Click(object sender, RoutedEventArgs e)
        { 
           // try
           // {
                mainWindow.CreateTable(SetMonth(), SetYear());
          //  }
         //   catch (Exception ex)
         //   {
          //      MessageBox.Show(ex.Message, "Исключение", MessageBoxButton.OK, MessageBoxImage.Warning);
          //  }
            Close();
        }

        public string SetMonth()
        {
            if (cbMonth.SelectedIndex < 9)
            {
                return "0" + Convert.ToString(cbMonth.SelectedIndex + 1);
            }
            else
            {
                return Convert.ToString(cbMonth.SelectedIndex + 1);
            }
            
        }
        public string SetYear()
        {
            
            return tbYear.Text;
        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //ComboBox comboBox = (ComboBox)sender;
            TextBlock selectedMonth = (TextBlock)cbMonth.SelectedItem;

            month = selectedMonth.Text;
        }
    }
}
