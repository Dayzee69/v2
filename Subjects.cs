using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.OleDb;

namespace Rib
{
    public class Subjects
    {
        public int ID { get; set; }
        public string Name { get; set; }
        public int Number { get; set; }
        public double InitalCost { get; set; }
        public double Depreciation { get; set; }
        public int Group { get; set; }
        public int MonthAdd { get; set; }
        public int YearAdd { get; set; }
        public int UsefulTime { get; set; }
        public double ResidualCost { get; set; }
        public int ResidualDate { get; set; }
        public string Account { get; set; }
        public string IsWorking { get; set; }
        public static List<Subjects> ReadDB(string year)
        {
            string connection = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=DataBase.mdb";

            var dbConnection = new OleDbConnection(connection);
            var dbCommand = dbConnection.CreateCommand();
            dbCommand.CommandText = "SELECT * FROM MainTable";

            dbConnection.Open();

            var dbReader = dbCommand.ExecuteReader();           

            List<Subjects> list = new List<Subjects>();
            while (dbReader.Read())
            {
                Subjects obj = new Subjects();
                obj.ID = int.Parse(dbReader["ID"].ToString());
                obj.Name = dbReader["Name"].ToString();
                obj.Number = int.Parse(dbReader["Individual number"].ToString());
                obj.InitalCost = Convert.ToDouble(dbReader["Initial cost"].ToString());
                obj.Depreciation = Convert.ToDouble(dbReader["Depreciation"].ToString());
                obj.Group = int.Parse(dbReader["Group Number"].ToString());
                obj.UsefulTime = int.Parse(dbReader["Useful time"].ToString());
                obj.MonthAdd = int.Parse(dbReader["Month add"].ToString());
                obj.YearAdd = int.Parse(dbReader["Year add"].ToString());                
                bool boolean = Convert.ToBoolean(dbReader["IsWorking"].ToString());
                if (boolean)
                    obj.IsWorking = "Эксплуатируется";
                else
                    obj.IsWorking = "Списано";

                obj.Account = dbReader["Account"].ToString();
                if(int.Parse(year) >= obj.YearAdd)
                list.Add(obj);
            }

            dbReader.Close();
            dbConnection.Close();
            return list;
        }

        public static List<Subjects> ReadMainTable()
        {
            string connection = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=DataBase.mdb";

            var dbConnection = new OleDbConnection(connection);
            var dbCommand = dbConnection.CreateCommand();
            dbCommand.CommandText = "SELECT * FROM MainTable";

            dbConnection.Open();

            var dbReader = dbCommand.ExecuteReader();

            List<Subjects> list = new List<Subjects>();
            while (dbReader.Read())
            {
                Subjects obj = new Subjects();
                obj.ID = int.Parse(dbReader["ID"].ToString());
                obj.Name = dbReader["Name"].ToString();
                obj.Number = int.Parse(dbReader["Individual number"].ToString());
                obj.InitalCost = Convert.ToDouble(dbReader["Initial cost"].ToString());
                obj.Depreciation = Convert.ToDouble(dbReader["Depreciation"].ToString());
                obj.Group = int.Parse(dbReader["Group Number"].ToString());
                obj.UsefulTime = int.Parse(dbReader["Useful time"].ToString());
                obj.MonthAdd = int.Parse(dbReader["Month add"].ToString());
                obj.YearAdd = int.Parse(dbReader["Year add"].ToString());
                bool boolean = Convert.ToBoolean(dbReader["IsWorking"].ToString());
                if (boolean)
                    obj.IsWorking = "Эксплуатируется";
                else
                    obj.IsWorking = "Списано";

                obj.Account = dbReader["Account"].ToString();                
                    list.Add(obj);
            }

            dbReader.Close();
            dbConnection.Close();
            return list;
        }


    }
}
