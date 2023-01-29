// DPSolidSProject.cpp
//
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Report
{
public:
    int PageSize = 100;
    string Text;
    string::iterator currentPos;

    void GoToFirstPage()
    {
        currentPos = Text.begin();
        cout << "go to first page\n";
    }
    void GoToLastPage()
    {
        currentPos = Text.end();
        cout << "go to last page\n";
    }
    void GoToPage(int page)
    {
        currentPos = Text.begin() + page * PageSize;
        cout << "go to " << page << " page\n";
    }
};

class Printer
{
public:
    void PrintReport(Report report)
    {
        cout << report.Text;
    }
};

class Product
{
public:
    string Title;
    int Price;
    Product(string title, int price) 
        : Title{ title }, Price{ price } {}
};

class StoreProductBad
{
    vector<Product> Products;
    void Process()
    {
        // input product
        cout << "input title of product: ";
        char title[50];
        cin.getline(title, 50);
        cout << "input price of product: ";
        int price;
        cin >> price;

        // validation
        if (price < 0)
            throw new exception("price not zero value!");

        // create product
        Product product(title, price);
        Products.push_back(product);

        // save to file
        ofstream file("products.txt");
        int i = 1;
        for (auto product : Products)
        {
            file << "#" << i++ << "\n";
            file << "Product title: " << product.Title << "\n";
            file << "Product price: " << product.Price << "\n";
        }
        file.close();
    }
};

class IProductInput
{
public:
    virtual vector<string> GetData() = 0;
};

class IProductValidate
{
public:
    virtual bool Validate(Product product) = 0;
};

class IProductCreate
{
public:
    virtual Product GetProduct(vector<string> data) = 0;
};

class IProductSave
{
public:
    virtual void Save(Product product, string fileName) = 0;
};

class StoreProduct
{
public:
    vector<Product> Products;
    
    IProductInput* inputer;
    IProductValidate* validator;
    IProductCreate* creator;
    IProductSave* saver;

    StoreProduct(IProductInput* inputer,
        IProductValidate* validator,
        IProductCreate* creator,
        IProductSave* saver)
        : inputer{ inputer }, validator{ validator }, creator{ creator }, saver{ saver } {};

    void Process()
    {
        vector<string> data = inputer->GetData();
        Product product = creator->GetProduct(data);
        if (validator->Validate(product))
        {
            Products.push_back(product);
            saver->Save(product, "products.txt");
        }
    }
};

class ProductInputer : public IProductInput
{
public:
    vector<string> GetData() override
    {
        vector<string> data;
        cout << "input title of product: ";
        char title[50];
        cin.getline(title, 50);
        cout << "input price of product: ";
        char price[50];
        cin.getline(price, 50);

        data.push_back(string(title));
        data.push_back(string(price));

        return data;
    }
};

class ProductCreater : public IProductCreate
{
public:
    Product GetProduct(vector<string> data) override
    {
        Product product(data[0], stoi(data[1]));
        return product;
    }
};

class ProductValidator : public IProductValidate
{
public:
    bool Validate(Product product) override
    {
        return product.Price >= 0;
    }
};

class ProductSaver : public IProductSave
{
public:
    void Save(Product product, string fileName) override
    {
        fstream file;
        file.open(fileName, ios::app);
        file << "Product title: " << product.Title << "\n";
        file << "Product price: " << product.Price << "\n";
        file.close();
    }
};

int main()
{
    StoreProduct store(new ProductInputer(), 
                       new ProductValidator(), 
                       new ProductCreater(), 
                       new ProductSaver());
    store.Process();
}
