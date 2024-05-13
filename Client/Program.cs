using DBConnector;

namespace Client
{
    internal class Program
    {
        private const string FileName = $"{CollectionName}/example.txt";
        private const string CollectionName = "collection";

        //попытка провести все это с файлами которых нет - нужно дописать ифы в серверной части 

        static void Main(string[] args)
        {
            DBConnection connector = new DBConnection("127.0.0.1", 55555);
            ListCollections(connector);
            CreateCollection(connector);
            ListCollections(connector);
            ListFiles(connector);
            AddFile(connector);
            ListFiles(connector);
            GetFile(connector);
            UpdateFile(connector);
            GetFile(connector);
            //DeleteFile(connector);
            ListFiles(connector);
            //DeleteCollection(connector);
            ListCollections(connector);
        }

        private static void ListCollections(DBConnection connector)
        {
            // Получаем список коллекций с сервера
            string collectionsList = connector.ListCollections();

            // Обрабатываем полученный список коллекций
            Console.WriteLine("Collections on server:");
            Console.WriteLine(collectionsList);
        }

        private static void DeleteCollection(DBConnection connector)
        {
            // Имя коллекции для удаления
            string collectionName = CollectionName;

            // Удаляем коллекцию на сервере
            string response = connector.DeleteCollection(collectionName);

            // Обрабатываем ответ
            Console.WriteLine("Server response: " + response);
        }

        private static void CreateCollection(DBConnection connector)
        {
            string newCollectionName = CollectionName;

            // Создаем новую коллекцию на сервере
            string response = connector.CreateCollection(newCollectionName);

            // Обрабатываем ответ
            Console.WriteLine("Server response: " + response);
        }

        private static void ListFiles(DBConnection connector)
        {
            string collectionName = CollectionName;

            // Получаем список файлов в коллекции с сервера
            string fileList = connector.ListFiles(collectionName);

            // Обрабатываем полученный список файлов
            Console.WriteLine("Files in collection " + collectionName + ":");
            Console.WriteLine(fileList);
        }

        private static void DeleteFile(DBConnection connector)
        {
            // Имя файла для удаления
            string fileName = FileName;

            // Удаляем файл на сервере
            string response = connector.DeleteFile(fileName);

            // Обрабатываем ответ
            Console.WriteLine("Server response: " + response);
        }

        private static void UpdateFile(DBConnection connector)
        {
            // Имя файла и новое содержимое
            string fileName = FileName;
            string newFileContent = "This is the updated content of the file.";

            // Обновляем файл на сервере
            string response = connector.UpdateFile(fileName, newFileContent);

            // Обрабатываем ответ
            Console.WriteLine("Server response: " + response);
        }

        private static void GetFile(DBConnection connector)
        {
            // Отправляем запрос на сервер
            string response = connector.GetFileContents(FileName);

            // Обрабатываем ответ
            Console.WriteLine("Server response: " + response);
        }

        private static void AddFile(DBConnection connector)
        {
            // Имя и содержимое файла
            string fileName = FileName;
            string fileContent = "This is the content of the file.";

            // Создаем файл на сервере
            string response = connector.CreateFile(fileName, fileContent);

            // Обрабатываем ответ
            Console.WriteLine("Server response: " + response);
        }
    }
}
