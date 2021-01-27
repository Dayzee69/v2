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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Data.OleDb;

namespace Rib
{
    
    public partial class MainWindow : Window
    {
        List<Subjects> subjects = new List<Subjects>();
        OrderWindow orderwindow;
        public MainWindow()
        {
            
            InitializeComponent();
        }       

        private void openOrder_Click(object sender, RoutedEventArgs e)
        {
            orderwindow = new OrderWindow(this);
            orderwindow.ShowDialog();
            
            
            
        }

        private void addSubject_Click(object sender, RoutedEventArgs e)
        {
            AddWindow addwindow = new AddWindow();
            addwindow.ShowDialog();
            
        }

        public void CreateTable(string SetMonth, string SetYear)
        {
 
            
            
            
            
                string strdate = SetMonth + SetYear;

            if (int.Parse(SetYear) < 2013)
            {

                throw new ArgumentException("Введенный год меньше 2013");

            }
                int date = int.Parse(strdate);

            
            
                string connection = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=DataBase.mdb";

                var dbConnection = new OleDbConnection(connection);
                var dbCommand = dbConnection.CreateCommand();
                dbCommand.CommandText = $"SELECT ID,[Residual cost],[Residual date] FROM Orders WHERE [Order date] = {date}";

                dbConnection.Open();

                var dbReader = dbCommand.ExecuteReader();
                if (!dbReader.HasRows)
                {
                    subjects = Subjects.ReadDB(SetYear);

                    dbReader.Close();
                    SetResidual(SetMonth, SetYear);

                    
                    dbCommand.CommandText = "INSERT INTO Orders ([ID], [Residual cost], [Residual date],[Order date])" +
                                    " VALUES (@ID, @ResidualCost, @ResidualDate, @date)";
                    dbCommand.Parameters.Add(new OleDbParameter("@ID", OleDbType.Integer));
                    dbCommand.Parameters.Add(new OleDbParameter("@ResidualCost", OleDbType.Double));
                    dbCommand.Parameters.Add(new OleDbParameter("@ResidualDate", OleDbType.Integer));
                    dbCommand.Parameters.Add(new OleDbParameter("@date", OleDbType.Integer));
                    foreach (var item in subjects)
                    {

                    dbCommand.Parameters[0].Value = item.ID;
                    dbCommand.Parameters[1].Value = item.ResidualCost;
                    dbCommand.Parameters[2].Value = item.ResidualDate;
                    dbCommand.Parameters[3].Value = date;
                    dbCommand.ExecuteNonQuery();

                    }


                Title = "Отчет за " + SetMonth + "." + SetYear;
                MyGrid2.IsEnabled = false;
                MyGrid2.Visibility = Visibility.Hidden;
                MyGrid.IsEnabled = true;
                MyGrid.Visibility = Visibility.Visible;
                MyGrid.ItemsSource = subjects;

                dbConnection.Close();
                }
                else
                {
                    subjects = Subjects.ReadDB(SetYear);


                    int[] arrID = new int[0];
                    int index = 0;
                
                    while (dbReader.Read())
                    {



                        for (int j = 0; j < subjects.Count; j++)
                            if (subjects[j].ID == int.Parse(dbReader["ID"].ToString()))
                            {
                                 subjects[j].ResidualCost = Convert.ToDouble(dbReader["Residual cost"].ToString());
                                 subjects[j].ResidualDate = int.Parse(dbReader["Residual date"].ToString());
                                 Array.Resize(ref arrID, arrID.Length + 1);
                                 arrID[index] = subjects[j].ID;
                                index++;
                            }
                        
                        
                    
                    }



                    dbReader.Close();
                    index = 0;
                if (arrID.Length != subjects.Count)
                {
                    for (int j = 0; j < subjects.Count; j++)
                    {
                        bool check = false;
                        for (int i = 0; i < arrID.Length; i++)
                        {
                            if (arrID[i]==subjects[j].ID)
                            {
                                check = true;
                                break;
                            }
                        }

                        if (!check)
                        {
                            int summMonth = int.Parse(SetMonth) - subjects[j].MonthAdd;

                            summMonth += (int.Parse(SetYear) - subjects[j].YearAdd) * 12;

                            subjects[j].ResidualCost = subjects[j].InitalCost - subjects[j].Depreciation * summMonth;
                            subjects[j].ResidualCost = Math.Round(subjects[j].ResidualCost, 2);
                            if (subjects[j].ResidualCost < 0)
                                subjects[j].ResidualCost = 0;
                            subjects[j].ResidualDate = subjects[j].UsefulTime - summMonth;
                            if (subjects[j].ResidualCost < 0)
                                subjects[j].ResidualCost = 0;

                            dbCommand.CommandText = "INSERT INTO Orders ([ID], [Residual cost], [Residual date],[Order date])" +
                                    " VALUES (@ID, @ResidualCost, @ResidualDate, @date)";

                            dbCommand.Parameters.Add(new OleDbParameter("@ID", OleDbType.Integer));
                            dbCommand.Parameters.Add(new OleDbParameter("@ResidualCost", OleDbType.Double));
                            dbCommand.Parameters.Add(new OleDbParameter("@ResidualDate", OleDbType.Integer));
                            dbCommand.Parameters.Add(new OleDbParameter("@date", OleDbType.Integer));

                            
                            dbCommand.Parameters[0].Value = subjects[j].ID;
                            dbCommand.Parameters[1].Value = subjects[j].ResidualCost;
                            dbCommand.Parameters[2].Value = subjects[j].ResidualDate;
                            dbCommand.Parameters[3].Value = date;
                            dbCommand.ExecuteNonQuery();
                            index++;
                            
                        }

                    }
                        
                    
                }
                     Title = "Отчет за " +SetMonth + "." + SetYear;
                     MyGrid2.IsEnabled = false;
                     MyGrid2.Visibility = Visibility.Hidden;
                     MyGrid.IsEnabled = true;
                     MyGrid.Visibility = Visibility.Visible;
                     MyGrid.ItemsSource = subjects;
                    
                    dbConnection.Close();
                }
            
           

        }
        void SetResidual(string m, string y)
        {


            int month = int.Parse(m);
            int year = int.Parse(y);
            string date = m + y;


            string connection = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=DataBase.mdb";

            var dbConnection = new OleDbConnection(connection);
            var dbCommandMod = dbConnection.CreateCommand();
            dbCommandMod.CommandText = $"SELECT ID,[Modification cost],[Modification month],[Modification year] FROM Modification WHERE [Modification year] <= {y}";

            dbConnection.Open();

            var dbReaderMod = dbCommandMod.ExecuteReader();

            if (!dbReaderMod.HasRows)
            {
                for (int i = 0; i < subjects.Count(); i++)
                {
                    int summMonth = month - subjects[i].MonthAdd;

                    summMonth += (year - subjects[i].YearAdd) * 12;

                    subjects[i].ResidualCost = subjects[i].InitalCost - subjects[i].Depreciation * summMonth;
                    subjects[i].ResidualCost = Math.Round(subjects[i].ResidualCost, 2);
                    if (subjects[i].ResidualCost < 0)
                        subjects[i].ResidualCost = 0;
                    subjects[i].ResidualDate = subjects[i].UsefulTime - summMonth;
                    if (subjects[i].ResidualDate < 0)
                        subjects[i].ResidualDate = 0;

                }
            }
            else
            {
                var dbCommandOrder = dbConnection.CreateCommand();
                
                int monthOrder = int.Parse(m)-1;
                if (monthOrder == 0)
                    monthOrder = 12;
                int yearOrder = int.Parse(y);

                string dateOrder = (monthOrder).ToString() + yearOrder.ToString();

                //MessageBox.Show(dateOrder);

                
                while (dbReaderMod.Read())
                {
                    for (int i = 0; i < subjects.Count(); i++)
                    {
                        if (int.Parse(dbReaderMod["ID"].ToString()) == subjects[i].ID && int.Parse(dbReaderMod["Modification month"].ToString()) < month)
                        {
                            int summMonth = month - subjects[i].MonthAdd;

                            //summMonth += (year - subjects[i].YearAdd) * 12;

                            dbCommandOrder.CommandText = $"SELECT [ID], [Residual cost], [Residual date] FROM Orders WHERE [Order date] = {dateOrder} AND [ID] = {subjects[i].ID}";

                            //
                            //
                            var dbReaderOrder = dbCommandOrder.ExecuteReader();

                            if (!dbReaderOrder.HasRows)
                                MessageBox.Show("Ошибка");

                            //MessageBox.Show("Ошибка");
                            dbReaderOrder.Read();
                            subjects[i].Depreciation = (Convert.ToDouble(dbReaderOrder["Residual cost"].ToString()) + Convert.ToDouble(dbReaderMod["Modification cost"].ToString()))
                              / int.Parse(dbReaderOrder["Residual date"].ToString());
                            subjects[i].Depreciation = Math.Round(subjects[i].Depreciation, 2);
                             //MessageBox.Show(subjects[i].Depreciation.ToString());


                            subjects[i].ResidualCost = Convert.ToDouble(dbReaderOrder["Residual cost"].ToString()) - subjects[i].Depreciation;

                            subjects[i].ResidualCost = Math.Round(subjects[i].ResidualCost, 2);
                            if (subjects[i].ResidualCost < 0)
                                subjects[i].ResidualCost = 0;
                            subjects[i].ResidualDate = int.Parse(dbReaderOrder["Residual date"].ToString()) - 1;
                            if (subjects[i].ResidualDate < 0)
                                subjects[i].ResidualDate = 0;

                            dbReaderOrder.Close();
                        }
                        else
                        {
                            int summMonth = month - subjects[i].MonthAdd;

                            summMonth += (year - subjects[i].YearAdd) * 12;

                            subjects[i].ResidualCost = subjects[i].InitalCost - subjects[i].Depreciation * summMonth;
                            subjects[i].ResidualCost = Math.Round(subjects[i].ResidualCost, 2);
                            if (subjects[i].ResidualCost < 0)
                                subjects[i].ResidualCost = 0;
                            subjects[i].ResidualDate = subjects[i].UsefulTime - summMonth;
                            if (subjects[i].ResidualDate < 0)
                                subjects[i].ResidualDate = 0;
                        }
                    }

                }

                dbReaderMod.Close();
                
               /* var dbCommand = dbConnection.CreateCommand();

                dbCommand.CommandText = $"UPDATE MainTable SET [Name]='{subject.Name}',[Individual number]={subject.Number},[Initial cost]={cost}," +
                $"[Depreciation]={depreciation},[Month add]={subject.MonthAdd},[Year add]={subject.YearAdd},[Useful time]={subject.UsefulTime}," +
                $"[IsWorking]={subject.IsWorking},[Account]='{subject.Account}',[Group Number]={subject.Group} WHERE ID = {subject.ID}";
                */

            }
        }

        private void openMainTable_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                CreateMainTable();
            }
            catch (Exception ex) {

                MessageBox.Show( ex.Message, "Исключение", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }

        public void CreateMainTable() {

            subjects = Subjects.ReadMainTable();
            Title = "Ведомость";
            MyGrid.IsEnabled = false;
            MyGrid.Visibility = Visibility.Hidden;
            MyGrid2.IsEnabled = true;
            MyGrid2.Visibility = Visibility.Visible;
            MyGrid2.ItemsSource = subjects;
        }

        private void update_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                int selectedColumn = 0;
                var selectedCell = MyGrid2.SelectedCells[selectedColumn];
                var cellContent = selectedCell.Column.GetCellContent(selectedCell.Item);
                EditWindow editwindow = new EditWindow(((cellContent as TextBlock).Text), this);
                editwindow.ShowDialog();
            }
            catch (Exception)
            {

            }
        }

        private void modification_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                int selectedColumn = 0;
                var selectedCell = MyGrid2.SelectedCells[selectedColumn];
                var cellContent = selectedCell.Column.GetCellContent(selectedCell.Item);
                ModificationWindow modwindow = new ModificationWindow(((cellContent as TextBlock).Text), this);
                modwindow.ShowDialog();
            }
            catch (Exception)
            {

            }
        }
    }
}
