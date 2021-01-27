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
    /// Логика взаимодействия для EditWindow.xaml
    /// </summary>
    public partial class EditWindow : Window
    {
        Subjects subject = new Subjects();
        MainWindow mainwindow;
        string month = "";
        public EditWindow(string strid,MainWindow window)
        {
            subject.ID = int.Parse(strid);
            mainwindow = window;
            InitializeComponent();
        }

        private void bUpdate_Click(object sender, RoutedEventArgs e)
        {
            try {
                Update();
                mainwindow.CreateMainTable();
            }
            catch (Exception ex) {

                MessageBox.Show(ex.Message, "Исключение", MessageBoxButton.OK ,MessageBoxImage.Warning);

            }
            Close();
        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            TextBlock selectedMonth = (TextBlock)cbMonth.SelectedItem;

            month = SetMonth(selectedMonth.Text);
        }
        void Update() {

            if (tbName.Text == "" || tbNumber.Text == "" || tbUsefulTime.Text == "" || tbInitialCost.Text == "" || tbYearAdd.Text == "" || 
                tbAccount.Text == "" || tbUsefulTime.Text =="")
            {
                throw new ArgumentException("Все поля обязательны для заполнения");

            }
            subject.Name = tbName.Text;
            subject.InitalCost = Convert.ToDouble(tbInitialCost.Text);
            if (subject.InitalCost < 0)
                throw new ArgumentException("Начальная стоимость не может быть меньше 0");
            subject.UsefulTime = int.Parse(tbUsefulTime.Text);
            if (subject.UsefulTime < 12)
                throw new ArgumentException("Срок эксплуатации не может быть меньше 12 месяцев");
            subject.Depreciation = subject.InitalCost / subject.UsefulTime;
            subject.Depreciation = Math.Round(subject.Depreciation, 2);
            TextBlock selectedMonth = (TextBlock)cbMonth.SelectedItem;
            month = SetMonth(selectedMonth.Text);
            subject.MonthAdd = int.Parse(month);
            subject.YearAdd = int.Parse(tbYearAdd.Text);
            if (subject.YearAdd < 2013)
                throw new ArgumentException("Введенный год меньше 2013");
            subject.Number = int.Parse(tbNumber.Text);
            if (subject.Number < 0)
                throw new ArgumentException("Индивидуальный номер долен быть больше 0 и состоят из 4 символов");
            subject.Account = tbAccount.Text;
            Regex rgx = new Regex("[0-9]{20}");
            if (!rgx.IsMatch(subject.Account))
                throw new ArgumentException("Лицевой счет должен состоять из 20 цифр");
            subject.Group = SetGroup(subject.UsefulTime);
            if (cbIsWorking.Text == "Эксплуатируется")
                subject.IsWorking = "true";
            else
                subject.IsWorking = "false";


            string connection = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=DataBase.mdb";

            var dbConnection = new OleDbConnection(connection);
            var dbCommand = dbConnection.CreateCommand();


            string cost = Convert.ToString(subject.InitalCost);
            string depreciation = Convert.ToString(subject.Depreciation);
            cost = cost.Replace(',', '.');
            depreciation = depreciation.Replace(',', '.');

            dbCommand.CommandText = $"UPDATE MainTable SET [Name]='{subject.Name}',[Individual number]={subject.Number},[Initial cost]={cost}," +
                $"[Depreciation]={depreciation},[Month add]={subject.MonthAdd},[Year add]={subject.YearAdd},[Useful time]={subject.UsefulTime}," +
                $"[IsWorking]={subject.IsWorking},[Account]='{subject.Account}',[Group Number]={subject.Group} WHERE ID = {subject.ID}";

            dbConnection.Open();

            dbCommand.ExecuteNonQuery();

            dbConnection.Close();
        }

        void LoadData()
        {

            string connection = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=DataBase.mdb";

            var dbConnection = new OleDbConnection(connection);
            var dbCommand = dbConnection.CreateCommand();
            dbCommand.CommandText = "SELECT [Name],[Individual number],[Initial cost],[Month add],[Year add],[Useful time],[Group Number],[IsWorking],[Account] " +
                $"FROM MainTable WHERE ID = {subject.ID}";
            dbConnection.Open();

            var dbReader = dbCommand.ExecuteReader();


            while (dbReader.Read())
            {
                subject.Name = dbReader["Name"].ToString();
                subject.Number = int.Parse(dbReader["Individual number"].ToString());
                subject.InitalCost = Convert.ToDouble(dbReader["Initial cost"].ToString());
                subject.UsefulTime = int.Parse(dbReader["Useful time"].ToString());
                subject.MonthAdd = int.Parse(dbReader["Month add"].ToString());
                subject.YearAdd = int.Parse(dbReader["Year add"].ToString());
                bool boolean = Convert.ToBoolean(dbReader["IsWorking"].ToString());
                if (boolean)
                    subject.IsWorking = "Эксплуатируется";
                else
                    subject.IsWorking = "Списано";
                subject.Account = dbReader["Account"].ToString();

            }

            dbReader.Close();
            dbConnection.Close();
        }
        void SetData()
        {
            tbName.Text = subject.Name;
            tbInitialCost.Text = Convert.ToString(subject.InitalCost);
            tbAccount.Text = subject.Account;
            cbMonth.SelectedIndex = subject.MonthAdd-1;
            tbNumber.Text = Convert.ToString(subject.Number);
            tbUsefulTime.Text = Convert.ToString(subject.UsefulTime);
            tbYearAdd.Text = Convert.ToString(subject.YearAdd);
            if(subject.IsWorking == "Эксплуатируется")
            cbIsWorking.SelectedIndex = 0;
            else
                cbIsWorking.SelectedIndex = 1;
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
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            LoadData();
            SetData();
            
        }
    }
}
