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
using System.Text.RegularExpressions;

namespace Rib
{
    /// <summary>
    /// Логика взаимодействия для AddWindow.xaml
    /// </summary>
    public partial class AddWindow : Window
    {
        Subjects newSubject = new Subjects();
        string month;
        public AddWindow()
        {
            InitializeComponent();
        }

        private void bAdd_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (tbName.Text == "" || tbIndivNumber.Text == "" || tbUsefulTime.Text == "" || tbInitCoast.Text == "" || tbYearAdd.Text == "" || tbAccount.Text == "")
                {
                    throw new ArgumentException("Все поля обязательны для заполнения");

                }

                    newSubject.Name = tbName.Text;

                newSubject.Number = int.Parse(tbIndivNumber.Text);
                if (newSubject.Number < 0)
                    throw new ArgumentException("Индивидуальный номер долен быть больше 0 и состоят из 4 символов");

                newSubject.UsefulTime = int.Parse(tbUsefulTime.Text);
                if (newSubject.UsefulTime < 12)
                    throw new ArgumentException("Срок эксплуатации не может быть меньше 12");

                newSubject.InitalCost = Convert.ToDouble(tbInitCoast.Text);
                newSubject.InitalCost = Math.Round(newSubject.InitalCost,2);
                if (newSubject.InitalCost < 0)
                    throw new ArgumentException("Начальная стоимость не может быть меньше 0");

                newSubject.Depreciation = newSubject.InitalCost / newSubject.UsefulTime;
                newSubject.Depreciation = Math.Round(newSubject.Depreciation, 2);
                newSubject.Group = SetGroup(newSubject.UsefulTime);
                newSubject.MonthAdd = int.Parse(month);
                newSubject.YearAdd = int.Parse(tbYearAdd.Text);
                if (newSubject.YearAdd < 2013)
                    throw new ArgumentException("Введенный год меньше 2013");

                newSubject.Account = tbAccount.Text;
                Regex rgx = new Regex("[0-9]{20}");
                if (!rgx.IsMatch(newSubject.Account))
                    throw new ArgumentException("Лицевой счет должен состоять из 20 цифр");

                

                AddSubject(newSubject);
                Close();
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message, "Исключение", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
            
            
        }

         private void AddSubject(Subjects subject)
        {
            string connection = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=DataBase.mdb";

            var dbConnection = new OleDbConnection(connection);
            var dbCommand = dbConnection.CreateCommand();
            

            dbConnection.Open();

            dbCommand.CommandText = $"INSERT INTO MainTable ([Name], [Individual number],[Initial cost],[Depreciation],[Month add],[Year add],[Useful time],[Group Number],[Account]) " +
                "VALUES (@Name, @Number,@InitalCost,@Depreciation,@MonthAdd,@YearAdd,@UsefulTime,@Group,@Account)";

            dbCommand.Parameters.Add(new OleDbParameter("@Name", OleDbType.VarChar));
             dbCommand.Parameters.Add(new OleDbParameter("@Number", OleDbType.Integer));
             dbCommand.Parameters.Add(new OleDbParameter("@InitalCost", OleDbType.Double));
             dbCommand.Parameters.Add(new OleDbParameter("@Depreciation", OleDbType.Double));
             dbCommand.Parameters.Add(new OleDbParameter("@MonthAdd", OleDbType.Integer));
             dbCommand.Parameters.Add(new OleDbParameter("@YearAdd", OleDbType.Integer));
             dbCommand.Parameters.Add(new OleDbParameter("@UsefulTime", OleDbType.Integer));
             dbCommand.Parameters.Add(new OleDbParameter("@Group", OleDbType.Integer));
             dbCommand.Parameters.Add(new OleDbParameter("@Account", OleDbType.VarChar));

             dbCommand.Parameters[0].Value = subject.Name;
             dbCommand.Parameters[1].Value = subject.Number;
             dbCommand.Parameters[2].Value = subject.InitalCost;
             dbCommand.Parameters[3].Value = subject.Depreciation;
             dbCommand.Parameters[4].Value = subject.MonthAdd;
             dbCommand.Parameters[5].Value = subject.YearAdd;
             dbCommand.Parameters[6].Value = subject.UsefulTime;
             dbCommand.Parameters[7].Value = subject.Group;
             dbCommand.Parameters[8].Value = subject.Account;
             
            dbCommand.ExecuteNonQuery();

            

            dbConnection.Close();
        }
        private int SetGroup(int check)
        {
            if (check > 12 && check <= 24)
            {
                return 1;
            }
            else if (check > 24 && check <= 36)
            {
                return 2;
            }
            else if (check > 36 && check <= 60)
            {
                return 3;
            }
            else if (check > 60 && check <= 84)
            {
                return 4;
            }
            else if (check > 84 && check <= 120)
            {
                return 5;
            }
            else if (check > 120 && check <= 195)
            {
                return 6;
            }
            else if (check > 195 && check <= 240)
            {
                return 7;
            }
            else if (check > 240 && check <= 300)
            {
                return 8;
            }
            else if (check > 300 && check <= 360)
            {
                return 9;
            }
            else if (check > 360)
            {
                return 10;
            }
            else return 0;

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
