#include <iostream>
#include <string>
#include <fstream>
#include "mysql.h"
using namespace std;

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "Jina";
const char* pw = "jina1008";
const char* db = "project2";

int main(void) {
	cin.tie(nullptr);
	ios_base::sync_with_stdio(false);

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		cout << "mysql_init() error!\n";

	connection = mysql_real_connect(&conn, host, user, pw, NULL, 3307, (const char*)NULL, 0);
	if (connection == NULL)
	{
		cout << mysql_errno(&conn) << " ERROR : " << mysql_error(&conn) << "\n";
		return 1;
	}
	mysql_query(connection, "CREATE DATABASE project2;");
	if (mysql_select_db(connection, db)) {
		cout << mysql_errno(&conn) << " ERROR : " << mysql_error(&conn) << "\n";
		return 1;
	}

	cout << "Connection Succeed\n";

	ifstream rFile("20160563.txt");
	if (rFile.is_open()) {
		char line[500];
		while (rFile.getline(line, 500)) {
			mysql_query(connection, line);
		}
		rFile.close();
	}
	else {
		cout << "No Such File\n";
		return 1;
	}

	if (mysql_select_db(&conn, db))
	{
		cout << mysql_errno(&conn) << " ERROR : " <<  mysql_error(&conn) << "\n";
		return 1;
	}
		

	int x, y;
	while (1) {
		cout << "\n\n";
		cout << "------- SELECT QUERY TYPES -------\n\n";
		cout << "\t1. TYPE I\n";
		cout << "\t2. TYPE II\n";
		cout << "\t3. TYPE III\n";
		cout << "\t4. TYPE IV\n";
		cout << "\t5. TYPE V\n";
		cout << "\t0. QUIT\n";
		cout << "Which type of query? ";
		cin >> x;
		printf("\n");

		if (x == 1) {
			cout << "---- TYPE I ----\n";
			while (1) {
				cout << "Input the number of truck : ";
				cin >> y;
				if (y != 1721) cout << "Truck " << y << " is not destroyed.\n\n";
				else if (y) {
					while (1) {
						cout << "\n---- Subtypes in TYPE I ----\n";
						cout << "\t1. TYPE I-1\n";
						cout << "\t2. TYPE I-2\n";
						cout << "\t3. TYPE I-3\n";
						cout << "Which type of query? ";
						cin >> y;
						if (y == 1) {
							cout << "\n---- TYPE I-1 ----\n";
							cout << "** Find all customers who had a package on the truck at the time of the crach. **\n";
							cout << "Customer Name : ";
							if (!mysql_query(connection, "select customer_name from customer, ship, truck where truck_id = 1721 and ship.package_id = truck.package_id and _time > '2021-05-01 07:00:00' and _time < '2021-05-01 10:20:00' and sender_id = customer_id;")) {
								sql_result = mysql_store_result(connection);
								while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								{
									cout << sql_row[0] << ", ";
								}
								cout << "\b\b.\n";
								mysql_free_result(sql_result);
							}
						}
						else if (y == 2) {
							cout << "\n---- TYPE I-2 ----\n";
							cout << "** Find all recipients who had a package on that truck at the time of the crash. **\n";
							cout << "Recipients Name : ";
							if (!mysql_query(connection, "select customer_name from customer, ship, truck where truck_id = 1721 and ship.package_id = truck.package_id and _time > '2021-05-01 07:00:00' and _time < '2021-05-01 10:20:00' and receiver_id = customer_id;")) {
								sql_result = mysql_store_result(connection);
								while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								{
									cout << sql_row[0] << ", ";
								}
								cout << "\b\b.\n";
								mysql_free_result(sql_result);
							}
						}
						else if (y == 3) {
							cout << "\n---- TYPE I-3 ----\n";
							cout << "** Find the last successful delivery by that truck prior to the crash. **\n";
							cout << "The Last Successful Delivery : ";
							if (!mysql_query(connection, "select S.package_id from ship as S, truck as T where S.delivered_date=(select max(delivered_date) from ship, truck where truck_id = 1721 and ship.package_id = truck.package_id and delivered_date < '2021-05-01 10:20:00') and T.truck_id = 1721 and S.package_id = T.package_id;")) {
								sql_result = mysql_store_result(connection);
								while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								{
									cout << sql_row[0] << "\n";
								}
								mysql_free_result(sql_result);
							}
						}
						else break;
					}
				}
				if (!y) break;
			}
		}
		else if (x == 2) {
			cout << "---- TYPE II ----\n";
			while (1) {
				string year;
				cout << "** Find the customer who has shipped the most packages in the past certain year. **\n";
				cout << "Which year? ";
				cin >> year;
				if (year=="0") break;
				cout << "Customer Name : ";
				string s = ("select customer_name \
					from customer, ship as S1 \
					where S1.shipping_date >= '" + year + "-01-01' and S1.shipping_date <= '" + year + "-12-31' and S1.sender_id = customer_id \
					group by S1.sender_id \
					having count(S1.package_id) = (select max(cnt) \
					from (select count(S2.package_id) as cnt \
					from ship as S2 \
					where S2.shipping_date >= '"+year+"-01-01' and S2.shipping_date <= '"+year+"-12-31' \
					group by S2.sender_id) A);");
				if (!mysql_query(connection, s.c_str())) {
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						cout << sql_row[0] << "\n";
					}
					mysql_free_result(sql_result);
				}
			}
		}
		else if (x == 3) {
			cout << "---- TYPE III ----\n";
			while (1) {
				string year;
				cout << "** Find the customer who has spent the most money on shipping in the past certain year. **\n";
				cout << "Which year? ";
				cin >> year;
				if (year=="0") break;
				cout << "Customer Name : ";
				string s = ("select customer_name \
					from customer, ship as S1 \
					where S1.shipping_date >= '" + year + "-01-01' and S1.shipping_date <= '" + year + "-12-31' and S1.sender_id = customer_id \
					group by S1.sender_id \
					having sum(S1.charge) = (select max(spent) \
					from (select sum(S2.charge) as spent \
					from ship as S2 \
					where S2.shipping_date >= '" + year + "-01-01' and S2.shipping_date <= '" + year + "-12-31' \
					group by S2.sender_id) A);");
				if (!mysql_query(connection, s.c_str())) {
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						cout << sql_row[0] << "\n";
					}
					mysql_free_result(sql_result);
				}
			}
		}
		else if (x == 4) {
			cout << "---- TYPE IV ----\n";
			cout << "** Find those packages that were not delivered within the promised time. **\n";
			if (!mysql_query(connection, "select ship.package_id\
				from ship, package\
				where ship.package_id = package.package_id and\
				((timeliness = 'overnight' and shipping_date < delivered_date)\
					or (timeliness = 'second day' and date_add(shipping_date, interval 1 day) < delivered_date));")) {
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					cout << sql_row[0] << ", ";
				}
				cout << "\b\b.\n";
				mysql_free_result(sql_result);
			}
		}
		else if (x == 5) {
			string name, month;
			cout << "---- TYPE V ----\n";
			cout << "** Generate the bill for each customer for the past certain month. Consider creating several types of bills. **\n";
			cout << "Customer Name : ";
			getline(cin, name);
			getline(cin, name);
			cout << "Which month(YYYY-MM)? ";
			cin >> month;
			cout << "Generating Bill..\n";
			ofstream wFile("bill.txt");
			if (wFile.is_open()) {
				string str = "Customer	 Address	Amount\n";
				wFile.write(str.c_str(), str.size());
				str = "select address, sum(charge)\
					from customer, ship\
					where customer_name = '" + name + "'\
					and customer_id = sender_id\
					and shipping_date >= '" + month + "-01'\
					and shipping_date <= '" + month + "-31';";
				if (!mysql_query(connection, str.c_str())) {
					sql_result = mysql_store_result(connection);
					str = "";
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						str += name + "  " + sql_row[0] + "  \\" + sql_row[1] + "\n";
					}
					wFile.write(str.c_str(), str.size());
					mysql_free_result(sql_result);
				}
				str = "------------------------------\n\
Itemized Billing List\n\
==================================================================\n\
| Package Number | Amount | Serivce Type | Timeliness of Delivery|\n\
==================================================================\n";
				wFile.write(str.c_str(), str.size());
				str = "select package.package_id, charge, package_type, timeliness\
					from customer, package, ship\
					where customer_name = '" + name + "'\
					and customer_id = sender_id\
					and shipping_date >= '" + month + "-01'\
					and shipping_date <= '" + month + "-31'\
					and package.package_id = ship.package_id;";
				if (!mysql_query(connection, str.c_str())) {
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						str = "";
						str = str + sql_row[0] + " \| " + sql_row[1] + " \| " + sql_row[2] + " \| " + sql_row[3] + "\n";
						wFile.write(str.c_str(), str.size());
					}
					mysql_free_result(sql_result);
				}
				wFile.close();
			}
			cout << "Generation Completed\n";
		}
		else break;
	}

	mysql_query(connection, "DELETE FROM ship");
	mysql_query(connection, "DELETE FROM plane;");
	mysql_query(connection, "DELETE FROM warehouse");
	mysql_query(connection, "DELETE FROM truck;");
	mysql_query(connection, "DELETE FROM tracking");
	mysql_query(connection, "DELETE FROM package;");
	mysql_query(connection, "DELETE FROM contracted");
	mysql_query(connection, "DELETE FROM not_contracted;");
	mysql_query(connection, "DELETE FROM customer;");

	mysql_query(connection, "DROP TABLE contracted;");
	mysql_query(connection, "DROP TABLE not_contracted;");
	mysql_query(connection, "DROP TABLE customer;");
	mysql_query(connection, "DROP TABLE package;");
	mysql_query(connection, "DROP TABLE plane;");
	mysql_query(connection, "DROP TABLE ship;");
	mysql_query(connection, "DROP TABLE tracking;");
	mysql_query(connection, "DROP TABLE truck;");
	mysql_query(connection, "DROP TABLE warehouse;");
	mysql_query(connection, "DROP TABLE project2;");

	mysql_close(connection);
	return 0;
}
