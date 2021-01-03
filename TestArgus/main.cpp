#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

map<string, cv::Mat> name_mat;


// загрузка рисунка с помощью OpenCV
cv::Mat OpenCV_loadFile(const char* filename){

    cv::Mat image = cv::imread(filename, 1);

    if (image.data != NULL){
        cout << "--file loaded:  " <<  filename << "\n\n";
        return image;
    }
    else
        cout << "error load file! " << "\n\n";
}

// сохранение рисунка с помощью OpenCV
void OpenCV_saveFile(const char* filename, const cv::Mat &img){
    if(cv::imwrite(filename, img))
        cout << "--file saved:  " <<  filename << "\n\n";
    else
        cout << "error save! " << "\n\n";;
}

// размытие рисунка с помощью OpenCV
void OpenCV_blur(const cv::Mat &src, cv::Mat &dst, cv::Size sz){
    cv::blur(src,dst,sz);
    cout << "--image blured " << "\n\n";
}

// изменение размера рисунка с помощью OpenCV
void OpenCV_resize(const cv::Mat &src, cv::Mat &dst, cv::Size ds){
    cv::resize(src,dst,ds);
    cout << "--image resized" << "\n\n";
}

// помощь по программе
void help(){
   cout << " === TestArgus help === \n\n";
   cout << " load, ld <name> <filename>  - load JPG image from file \n";
   cout << "   <name>           - internal name \n";
   cout << "   <filename>       - file name \n\n";

   cout << " store, s <name> <filename>  - save image file to JPG format \n";
   cout << "   <name>           - internal name \n";
   cout << "   <filename>       - file name for save \n\n";

   cout << " blur <from_name>            - blur image \n";
   cout << "   <from_name>      - internal name \n";
   cout << "   <to_name>        - internal name \n";
   cout << "   <size>           - image size \n\n";

   cout << " resize <from_name> <to_name> <new_width> <new_height>  - resize image \n";
   cout << "   <from_name>      - internal name \n";
   cout << "   <to_name>        - internal name \n";
   cout << "   <new_width>      - new width \n";
   cout << "   <new_height>     - new height \n\n";

   cout << " exit, quit, q      - terminate program \n\n";
   cout << " ======================= \n\n";
}


// arguments parser
void argvParsed(const vector<string> &arg){
    int id;

    for (id=0; id < arg.size(); id++){

        if ((arg[id]=="help")||(arg[id] == "h")){
            help();
            continue;
        }

        if ((arg[id]=="load")||(arg[id]=="ld")){
            if ( (++id<arg.size()) && (arg[id]!="")){
                if (name_mat.find(arg[id])==name_mat.end()){
                    name_mat[arg[id]] = NULL;
                }
                if((++id<arg.size())&&(arg[id]!="")){
                    // name_mat[arg[id-1]] = OpenCV_loadFile(arg[id].toUtf8().data());
                    name_mat[arg[id-1]] = OpenCV_loadFile(arg[id].data());
                    continue;
                }
                else {
                    cout << "No argument "<< id <<"\n";
                    break;
                }
            }
            else {
                cout << "No argument "<< id <<"\n";
                break;
            }
        }

        if ((arg[id]=="store")||(arg[id]=="s")){
            if ( ++id<arg.size() ){
                if ( name_mat.find(arg[id])==name_mat.end() ){
                    cout << "No argument "<< id <<"\n";
                    break;
                }
                if( ++id < arg.size() ){
                    OpenCV_saveFile(arg[id].data(), name_mat[arg[id-1]]);
                    continue;
                }
                else {
                    cout << "No argument "<< id <<"\n";
                    break;
                }
            }
            else {
                cout << "No argument "<< id <<"\n";
                break;
            }
        }

        if (arg[id]=="blur"){
            if ( ++id<arg.size() ){
                if ( name_mat.find(arg[id])==name_mat.end() ){
                    cout << "No argument "<< id <<"\n";
                    break;
                }
                string key_src = arg[id];

                if( ++id < arg.size() ){
                    if ( name_mat.find(arg[id])==name_mat.end() ){
                        name_mat[arg[id]] = NULL;
                    }
                }
                string key_dst = arg[id];

                cv::Size reg_size;  // размер сглаживаемой области

                if( ++id < arg.size() ){
                    reg_size.width = atoi(arg[id].c_str());
                }
                else {
                    cout << "No argument "<< id <<"\n";
                    break;
                }

                if( ++id < arg.size() ){
                    reg_size.height = atoi(arg[id].c_str());
                }
                else {
                    cout << "No argument "<< id <<"\n";
                    break;
                }

                OpenCV_blur(name_mat[key_src], name_mat[key_dst], reg_size);
                continue;
            }
        }

        if (arg[id]=="resize"){
            if ( ++id<arg.size() ){
                if ( name_mat.find(arg[id])==name_mat.end() ){
                    cout << "No argument "<< id <<"\n";
                    break;
                }
                string key_src{arg[id]};

                if( ++id<arg.size() ){
                    if ( name_mat.find(arg[id])==name_mat.end() ){
                        name_mat[arg[id]] = NULL;
                    }
                    string key_dst{arg[id]};

                    cv::Size ds;

                    if( ++id<arg.size() ){
                        ds.width = atoi(arg[id].c_str());

                        if( ++id<arg.size() ){
                            ds.height = atoi(arg[id].c_str());
                            OpenCV_resize(name_mat[key_src],name_mat[key_dst],ds);
                            continue;
                        }
                        else {
                            cout << "No argument "<< id <<"\n";
                            break;
                        }
                    }
                    else {
                        cout << "No argument "<< id <<"\n";
                        break;
                    }
                }
                else {
                    cout << "No argument "<< id <<"\n";
                    break;
                }
            }
            else{
                cout << "No argument "<< id <<"\n";
                break;
            }
        }

        if ((arg[id]=="exit")||(arg[id]=="quit")||(arg[id]=="q")){
            exit(0);
        }

    }  // end for

    cout << "For exit press Enter" <<"\n";
    char ch;
    cin.get();

    exit(0);
}


int main(int argc, char *argv[])
{
    vector<string> strl;

    for (int i=1; i<argc; i++)  {
        strl.push_back(argv[i]);
    }

    argvParsed(strl);

    return(0);
}


