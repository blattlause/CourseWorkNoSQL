using System.Net;
using System.Net.Sockets;
using System.Text;

namespace DBConnector
{
    //этот класс отправляет запрсы по сети для сервера и получает ответ 
    public class DBConnection
    {
        // IP adress server
        private string _address;
        private int _port;

        public DBConnection(string address, int port)
        {
            _address = address;
            _port = port;
        }


        public int Port { get => _port ; set => _port = value; }
        public string Address { get => _address; set => _address = value; }

        //запрос на сервер через TCP
        private string SendRequest(string request)
        {
            try
            {
                using (TcpClient client = new TcpClient(Address, Port))
                {
                    NetworkStream stream = client.GetStream();

                    byte[] data = Encoding.UTF8.GetBytes(request);

                    stream.Write(data, 0, data.Length);

                    // Читаем ответ от сервера построчно
                    StreamReader reader = new StreamReader(stream, Encoding.UTF8);
                    StringBuilder response = new StringBuilder();
                    string line;
                    while ((line = reader.ReadLine()) != null)
                    {
                        response.AppendLine(line);
                    }

                    stream.Close();
                    return response.ToString();
                }
            }
            catch (Exception ex)
            {
                return "Error: " + ex.Message;
            }
        }



        public string CreateFile(string fileName, string fileContent)
        {
            try
            {
                // Формируем запрос для создания файла
                string request = $"CREATE_FILE {fileName} {fileContent}";

                // Отправляем запрос на сервер и получаем ответ
                return SendRequest(request);
            }
            catch (Exception ex)
            {
                // Обработка ошибок
                return "Error: " + ex.Message;
            }
        }

        public string UpdateFile(string fileName, string newFileContent)
        {
            try
            {
                // Формируем запрос для обновления файла
                string request = $"UPDATE_FILE {fileName} {newFileContent}";

                // Отправляем запрос на сервер и получаем ответ
                return SendRequest(request);
            }
            catch (Exception ex)
            {
                // Обработка ошибок
                return "Error: " + ex.Message;
            }
        }

        public string DeleteFile(string fileName)
        {
            try
            {
                // Формируем запрос для удаления файла
                string request = $"DELETE_FILE {fileName}";

                // Отправляем запрос на сервер и получаем ответ
                return SendRequest(request);
            }
            catch (Exception ex)
            {
                // Обработка ошибок
                return "Error: " + ex.Message;
            }
        }

        public string GetFileContents(string fileName)
        {
            try
            {
                // Формируем запрос для получения содержимого файла
                string request = $"GET_FILE {fileName}";

                // Отправляем запрос на сервер и получаем ответ
                return SendRequest(request);
            }
            catch (Exception ex)
            {
                // Обработка ошибок
                return "Error: " + ex.Message;
            }
        }

        public string ListFiles(string collectionName)
        {
            try
            {
                // Формируем запрос для получения списка файлов в коллекции
                string request = $"LIST_FILES {collectionName}";

                // Отправляем запрос на сервер и получаем ответ
                return SendRequest(request);
            }
            catch (Exception ex)
            {
                // Обработка ошибок
                return "Error: " + ex.Message;
            }
        }

        public string CreateCollection(string collectionName)
        {
            try
            {
                // Формируем запрос для создания коллекции
                string request = $"CREATE_COLLECTION {collectionName}";

                // Отправляем запрос на сервер и получаем ответ
                return SendRequest(request);
            }
            catch (Exception ex)
            {
                // Обработка ошибок
                return "Error: " + ex.Message;
            }
        }

        public string DeleteCollection(string collectionName)
        {
            try
            {
                // Формируем запрос для удаления коллекции
                string request = $"DELETE_COLLECTION {collectionName}";

                // Отправляем запрос на сервер и получаем ответ
                return SendRequest(request);
            }
            catch (Exception ex)
            {
                // Обработка ошибок
                return "Error: " + ex.Message;
            }
        }

        public string ListCollections()
        {
            try
            {
                // Формируем запрос для получения списка коллекций
                string request = "LIST_COLLECTIONS";

                // Отправляем запрос на сервер и получаем ответ
                return SendRequest(request);
            }
            catch (Exception ex)
            {
                // Обработка ошибок
                return "Error: " + ex.Message;
            }
        }
    }
}
