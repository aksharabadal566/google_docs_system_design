#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

// abstraction of class document element 
class docele{
public:
    virtual string render() = 0;
    virtual ~docele() = default;   // IMPORTANT: virtual destructor
};

// text elements 
class textelement: public docele{
private:
    string text;

public:
    textelement(const string& text){
        this->text = text;
    }

    string render() override{
        return text;
    }
};

// image elements 
class imageelement: public docele{
private:
    string path;

public:
    imageelement(const string& path){
        this->path = path;
    }

    string render() override{
        return "[image: " + path + "]";
    }
};

// for new line
class newline : public docele{
public:
    string render() override{
        return "\n";   // fixed
    }
};

// document class which will hold our collection of elements
class Document{
private:
    vector<docele*> docelements;

public:
    void addele(docele* element){
        docelements.push_back(element);
    }

    string render(){
        string result;
        for(auto element : docelements){
            result += element->render();
        }
        return result;
    }

    ~Document(){   // clean up memory
        for(auto element : docelements){
            delete element;
        }
    }
};

// persistent interface 
class persistence{
public:
    virtual void save(const string& data) = 0;
    virtual ~persistence() = default;   // IMPORTANT
};

// file storage implementation of persistence 
class filestorage : public persistence{
public:
    void save(const string& data) override{
        ofstream outfile("document.txt");
        if(outfile){
            outfile << data;
            outfile.close();
            cout << "doc saved to document.txt" << endl;
        }
        else{
            cout << "error: unable to open file for writing" << endl;
        }
    }
};

// placeholder db implementation 
class dbstorage: public persistence{
public:
    void save(const string& data) override{
        cout << "Saving document to database (placeholder)\n";
    }
};

class doceditor{
private:
    Document* document;
    persistence* storage;

public:
    doceditor(Document* document, persistence* storage){
        this->document = document;
        this->storage = storage;
    }

    void addtext(const string& text){
        document->addele(new textelement(text));
    }

    void addimage(const string& path){
        document->addele(new imageelement(path));
    }

    void addnewline(){
        document->addele(new newline());
    }

    void save(){
        string rendered = document->render();
        storage->save(rendered);
    }
};

// main function to test
int main(){
    Document doc;
    filestorage fileStorage;

    doceditor editor(&doc, &fileStorage);

    editor.addtext("Hello World");
    editor.addnewline();
    editor.addimage("photo.png");

    editor.save();

    return 0;
}
