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
using System.Data.OleDb;

namespace Rib
{
    /// <summary>
    /// Логика взаимодействия для ModificationWindow.xaml
    /// </summary>
    public partial class ModificationWindow : Window
    {
        Subjects subject = new Subjects();
        MainWindow mainwindow;
        string month = "";
        public ModificationWindow(string strid, MainWindow window)
        {
            subject.ID = int.Parse(strid);
            mainwindow = window;
            InitializeComponent();
        }

        private void bMod_Click(object sender, RoutedEventArgs e)
        {
            //try
           // {
                ModificationAdd();
                Close();

            //} catch(Exception ex){


              //  MessageBox.Show(ex.Message, "Исключение", MessageBoxButton.OK, MessageBoxImage.Warning);
           // }
        }

        void ModificationAdd()
        {

            if (tbName.Text == "" || tbCost.Text == "" || tbYear.Text == "")
            {
                throw new ArgumentException("Все поля обязательны для заполнения");

            }

           
                subject.Name = tbName.Text;
                subject.InitalCost = Convert.ToDouble(tbCost.Text);
            subject.InitalCost = Math.Round(subject.InitalCost,2);
            if (subject.InitalCost < 0)
                throw new ArgumentException("Начальная стоимость не может быть меньше 0");

            string connection = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=DataBase.mdb";

            var dbConnection = new OleDbConnection(connection);
            var dbCommand = dbConnection.CreateCommand();

            string cost = Convert.ToString(subject.InitalCost);
            cost = cost.Replace(',', '.');


            
            
            if (int.Parse(tbYear.Text) < 2013)
                throw new ArgumentException("Введенный год меньше 2013");


            dbConnection.Open();

            

            dbCommand.CommandText = $"INSERT INTO Modification ([ID], [Modification name],[Modification cost],[Modification month],[Modification year]) " +
                $"VALUES ({subject.ID},'{subject.Name}',{cost},{month},{tbYear.Text})";

            dbCommand.ExecuteNonQuery();



            dbConnection.Close();
        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            TextBlock selectedMonth = (TextBlock)cbMonth.SelectedItem;

            month = SetMonth(selectedMonth.Text);
        }
        string SetMonth(string month)
        {
            switch (month)
            {
                case "Январь": return "01";

                case "Февраль": return "02";

                case "Март": return "03";

                case "Апрель": return "04";

                case "Май": return "05";

                case "Июнь": return "06";

                case "Июль": return "07";

                case "Август": return "08";

                case "Сентябрь": return "09";

                case "Октябрь": return "10";

                case "Ноябрь": return "11";

                case "Декабрь": return "12";

                default: return "0";
            }
        }

    }
}
