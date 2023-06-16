#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<fstream>
#include<sstream>
#include<assert.h>
using namespace std;

vector<string> split_string(string line,string delimiter=","){
    vector<string>strs;
    int pos=0;
    string sub;
    while((pos=(int)line.find(delimiter))!=-1)
    {
        sub=line.substr(0,pos);
        strs.push_back(sub);
        line.erase(0,pos + delimiter.length());
    }
    strs.push_back(line);
    return strs;
}

int Toint(string sub){
    istringstream iss(sub);
    int num;
    iss>>num;
    return num;
}

vector<string> read_file_lines(const string &path){
    vector<string>lines;

    ifstream filehandler(path);
    if(!filehandler)
    {
        cout<<"can not open the file \n";
        return lines;
    }

       string line;
       while(getline(filehandler,line))
         {
        if(line.size()==0)
            continue;
        lines.push_back(line);
         }


    filehandler.close();
    return lines;
}

void write_file_lines(string path,vector<string>lines,bool append=true){
    auto status=ios::in|ios::out|ios::app;

    if(!append)
        status=ios::in|ios::out|ios::trunc;
    fstream filehandler(path.c_str(),status);

    if(!filehandler)
    {
        cout<<"\n\ncan not open the file \n\n";
        return;
    }

    for(auto line:lines)
        filehandler<<line<<"\n";
    filehandler.close();
}

int Readint(int low,int high)
{
    cout<<"\nEnter number in range :"<<low<<"-"<<high;
    int value;
    cin>>value;
    if(value>=low&&value<=high)
        return value;
    else
    {
        cout<<"invalid choice....try again \n";
        return Readint(low,high);
    }

}

int show_read_menue(vector<string>choices)
{
    cout<<"\nMenu\n";
    for(int ch=0;ch<(int)choices.size();++ch)
    {
        cout<<"\t"<<ch+1<<": "<<choices[ch]<<endl;
    }
    return Readint(1,choices.size());
}

struct Question{
    int question_id;
    int parent_question_id;
    int from_user_id;
    int to_user_id;
    int is_anonymous_question;
    string question_text;
    string answer_text;

    Question(){
        question_id=parent_question_id=from_user_id=to_user_id=is_anonymous_question=-1;
    }

    Question(string line){
        vector<string>substrs=split_string(line);
        assert(substrs.size()==7);

        question_id=Toint(substrs[0]);
        parent_question_id=Toint(substrs[1]);
        from_user_id=Toint(substrs[2]);
        to_user_id=Toint(substrs[3]);
        is_anonymous_question=Toint(substrs[4]);
        question_text=substrs[5];
        answer_text=substrs[6];
    }

    void print_to_questions(){
        string perfix="";
        if(parent_question_id!=-1)
        {
            perfix="\tthread: ";
        }
        cout<<perfix<<"Question id("<<question_id<<")";
        if(is_anonymous_question==0)
            cout<<" from user id("<<from_user_id<<")";
        cout<<"\t Question: "<<question_text<<endl;
        if(answer_text!="")
            cout<<perfix<<"Answer: "<<answer_text<<"\n";
        cout<<endl;

    }

    void print_from_question(){
        cout<<"Question id ("<<question_id<<")";
        if(is_anonymous_question==0)
            cout<<"!AQ to user id("<<to_user_id<<")";
        else
            cout<<"to user id("<<to_user_id<<")";
        cout<<"\tQuestion: "<<question_text<<"\t";
        if(answer_text!="")
            cout<<"Answer: "<<answer_text<<endl;
        else
            cout<<"NOt answerd yet\n";


    }
    void print(){
        cout<<"Question id ("<<question_id<<")";
        if(is_anonymous_question!=0)
            cout<<"from user id ("<<from_user_id<<")";
        cout<<"\t Question: "<<question_text<<"\n";
        if(answer_text!="")
            cout<<"\tAnswer: "<<answer_text<<endl;
    }

    void print_list_feed(){
        if(parent_question_id!=-1)
            cout<<"Thread parent question id ("<<parent_question_id<<") ";
        cout<<"Question id ("<<question_id<<")";
        if(is_anonymous_question!=0)
            cout<<"from user id("<<from_user_id<<")";
        cout<<"to user id("<<to_user_id<<")";
        cout<<"\t Question: "<<question_text<<"\n";
        if(answer_text!="")
            cout<<"Answer: "<<answer_text<<endl;

    }

    string Tostring(){
      ostringstream oss;
      oss<<question_id<<","<<parent_question_id<<","<<from_user_id<<","<<to_user_id<<","<<is_anonymous_question<<","<<question_text<<","<<answer_text;
      return oss.str();
    }

};

struct User{
    int user_id;
    string username;
    string password;
    string name;
    string email;
    int allow_for_anonymous_questions;

    vector<int>questions_ids_from_me;
    map<int,vector<int>>questionid_questionidsthread_to_map;

    User(){
        user_id=allow_for_anonymous_questions=-1;
    }

    User(string line){
        vector<string>substrs=split_string(line);
        assert(substrs.size()==6);
        user_id=Toint(substrs[0]);
        username=substrs[1];
        password=substrs[2];
        name=substrs[3];
        email=substrs[4];
        allow_for_anonymous_questions=Toint(substrs[5]);
    }
    string Tostring(){
        ostringstream oss;
        oss<<user_id<<","<<username<<","<<password<<","<<name<<","<<email<<","<<allow_for_anonymous_questions;
        return oss.str();
    }


};

struct Questions_manager{
    Question question;
    map<int,Question> questionid_questionobject_map;
    //for all users
    map<int,vector<int>>questionid_questionidsthread_to_map;

    int last_id;

    Questions_manager(){
        last_id=0;
    }


    void print_To_questions(User &user)
    {
        cout<<"\n";
       if(user.questionid_questionidsthread_to_map.size()==0)
        cout<<"No questions\n";
       for(auto &pair:user.questionid_questionidsthread_to_map)
       {
           for(auto &question_id:pair.second)
           {
               Question &question=questionid_questionobject_map[question_id];
               question.print_to_questions();
           }
       }
       cout<<"\n";
    }

    void print_From_question(User &user){
        cout<<"\n";
        if(user.questions_ids_from_me.size()==0)
            cout<<"NO questions \n";
        for(auto &question_id:user.questions_ids_from_me)
        {
            Question question=questionid_questionobject_map[question_id];
            question.print_from_question();
        }
        cout<<endl;

    }

    void fill_user_questions(User &user){
        user.questions_ids_from_me.clear();
        user.questionid_questionidsthread_to_map.clear();

        for(auto &pair:questionid_questionidsthread_to_map)
        {
            for(auto &question_id:pair.second)
            {
                Question &question = questionid_questionobject_map[question_id];
                if(question.from_user_id==user.user_id)
                    user.questions_ids_from_me.push_back(question_id);
                if(question.to_user_id==user.user_id)
                {
                    if(question.parent_question_id==-1)
                        user.questionid_questionidsthread_to_map[question_id].push_back(question_id);
                    else
                        user.questionid_questionidsthread_to_map[question.parent_question_id].push_back(question_id);
                }
            }
        }
        }


        int Read_questionid_thread(User &user){
            int question_id;
            cout<<"For thread question enter question id or -1 for a new question \n";
            cin>>question_id;
            if(question_id==-1)
                return question_id;
           if(!questionid_questionobject_map.count(question_id))
           {
               cout << "No thread question with such ID. Try again\n";
               return Read_questionid_thread(user);
           }
           return question_id;

        }

        void ask_question(pair<int,int>u,User &user)
        {
            Question question;

            if(u.second==0)
            {
              cout<<"Note: Anonymous questions are not allowed for this user \n";
              question.is_anonymous_question=0;
            }
            else
            {
                cout<<"Is anonymous question ? (0,1)\n";
                cin>>question.is_anonymous_question;
            }
            question.parent_question_id = Read_questionid_thread(user);
            cout<<"Enter question text :\n";
            getline(cin, question.question_text);
		    getline(cin, question.question_text);

            question.from_user_id=user.user_id;
            question.to_user_id=u.first;
            question.question_id=++last_id;
            question.answer_text=="";
            questionid_questionobject_map[question.question_id]=question;
            if(question.parent_question_id==-1)
                questionid_questionidsthread_to_map[question.question_id].push_back(question.question_id);
            else
                questionid_questionidsthread_to_map[question.parent_question_id].push_back(question.question_id);


        }

        int Read_questionid_any(User &user){
            cout<<"Enter question id or -1 to cancel \n";
            int question_id;
            cin>>question_id;
            if(question_id==-1)
                return question_id;
            if(!questionid_questionobject_map.count(question_id))
            {
                cout<<"NO question with such id...try again \n";
                return Read_questionid_any(user);
            }
            Question question=questionid_questionobject_map[question_id];
            if(question.to_user_id!=user.user_id)
            {
                cout<<"Invalid question id...try again\n";
                return Read_questionid_any(user);
            }
            return question_id;
        }

        void answer_question(User &user){
            int question_id=Read_questionid_any(user);
            if(question_id==-1)
                return;
            Question question=questionid_questionobject_map[question_id];
            question.print();

            if(question.answer_text!="")
                cout<<"warning: Already answered ,Answer will be updated \n";
            cout<<"Enter Answer:\n";
            getline(cin, question.answer_text);	// read last enter
		    getline(cin, question.answer_text);
        }

        void delete_question(User &user)
        {
            int question_id=Read_questionid_any(user);
            if(question_id==-1)
                return;
            vector<int>ids_to_remove;
            if(questionid_questionidsthread_to_map.count(question_id))
            {
                ids_to_remove=questionid_questionidsthread_to_map[question_id];
                questionid_questionidsthread_to_map.erase(question_id);
            }
            else
            {
                ids_to_remove.push_back(question_id);
            }

            //search for the thread that the question is apart of
            for(auto &pair:questionid_questionidsthread_to_map)
            {
                vector<int> &vec=pair.second;
                for(int pos=0;pos<(int)vec.size();++pos)
                {
                    if(question_id==vec[pos]){
                        vec.erase(vec.begin()+pos);
                        break;
                    }
                }
            }
            for(auto &id:ids_to_remove)
                questionid_questionobject_map.erase(id);
        }

        void listfeed(){
            for(auto &pair:questionid_questionobject_map)
            {
                Question &question=pair.second;
                if(pair.second.answer_text=="")
                    continue;

                question.print_list_feed();
            }

        }

        void Update_database(){
            vector<string>lines;
            for(auto &pair:questionid_questionobject_map)
                lines.push_back(pair.second.Tostring());
            write_file_lines("questions.txt",lines,false);
        }

        void load_database(){
            last_id=0;
            questionid_questionidsthread_to_map.clear();
            questionid_questionobject_map.clear();

            vector<string>lines= read_file_lines("questions.txt");
            for(auto &line:lines)
                Question question(line);

            questionid_questionobject_map[question.question_id]=question;
            last_id=max(last_id,question.question_id);

            if(question.parent_question_id==-1)
                questionid_questionidsthread_to_map[question.question_id].push_back(question.question_id);
            else
                questionid_questionidsthread_to_map[question.parent_question_id].push_back(question.question_id);

        }


};

struct Users_manager{
    User current_user;
    map<string,User>username_userobject_map;
    int last_id=0;

    Users_manager(){
        last_id=0;
    }

    void Access_system(){
        int choice=show_read_menue({"login","signup"});
        if(choice==1)
            do_login();
        else
            do_signup();
    }

    void do_login(){
        load_database();
        while(true){
        cout<<"Enter username and password : ";
        cin>>current_user.username>>current_user.password;

            if(username_userobject_map.count(current_user.username)==0)
            {
                cout<<"\nusername or password is not correct...try again \n\n";
                continue;}
        User user_exist=username_userobject_map[current_user.username];
        if(user_exist.password!=current_user.password)
        {
            cout<<"\nusername or password is not correct....try again\n\n ";
            continue;
        }
        current_user=user_exist;
        break;
       }

        }


    void do_signup(){
        while(true){
        cout<<"Enter username (nospaece)\n";
        cin>>current_user.username;
            if(username_userobject_map.count(current_user.username))
                cout<<"username already used..try again \n";
            else
                break;

        }

        cout<<"Enter password : ";
        cin>>current_user.password;
        cout<<"Enter Name : ";
        cin>>current_user.name;
        cout<<"Enter email : ";
        cin>>current_user.email;
        cout<<"Allow anonymous questions ? (0,1)";
        cin>>current_user.allow_for_anonymous_questions;
        current_user.user_id=++last_id;
        username_userobject_map[current_user.username]=current_user;
        Update_database(current_user);
        }


        pair<int,int> Read_user_id(){
            cout<<"Enter user id or -1 to cancel :";
            int user_id;
            cin>>user_id;

            if(user_id==-1)
                return make_pair(-1,-1);

            for(auto &pair: username_userobject_map)
                {
                    if(pair.second.user_id==user_id)
                        return make_pair(user_id,pair.second.allow_for_anonymous_questions);
                }

            cout<<"there is not user with such user id.....try again\n";
            return Read_user_id();
        }

        void listusers_names_ids(){
            for(auto &pair:username_userobject_map)
            {
                cout<<"ID: "<<pair.second.user_id<<"\t\t Name: "<<pair.second.name<<endl;
            }
        }

        void Update_database(User &user){
            string line = user.Tostring();
            vector<string> lines(1,line);
            write_file_lines("users.txt",lines);
        }

        void load_database(){
            last_id=0;
            username_userobject_map.clear();

            vector<string>lines= read_file_lines("users.txt");
            for(auto &line:lines)
            {
                User user(line);
                username_userobject_map[user.username]=user;
                last_id=max(last_id,user.user_id);
            }
        }

};

struct AskMeSystem{
    Questions_manager questions_manager;
    Users_manager users_manager;
    void load_database(bool fillusers_questions=false){
        users_manager.load_database();
        questions_manager.load_database();

        if(fillusers_questions)
            questions_manager.fill_user_questions(users_manager.current_user);

    }

    void run(){
        load_database();
        users_manager.Access_system();
        questions_manager.fill_user_questions(users_manager.current_user);

        vector<string> menue;
		menue.push_back("Print Questions To Me");
		menue.push_back("Print Questions From Me");
		menue.push_back("Answer Question");
		menue.push_back("Delete Question");
		menue.push_back("Ask Question");
		menue.push_back("List System Users");
		menue.push_back("Feed");
		menue.push_back("Logout");

		while(true){
          int choice=show_read_menue(menue);
          load_database(true);
          if(choice==1)
            questions_manager.print_To_questions(users_manager.current_user);
          else if(choice==2)
            questions_manager.print_From_question(users_manager.current_user);
          else if(choice==3){

            questions_manager.answer_question(users_manager.current_user);
            questions_manager.Update_database();
          }
          else if(choice==4)
          {
              questions_manager.delete_question(users_manager.current_user);
              questions_manager.fill_user_questions(users_manager.current_user);
              questions_manager.Update_database();
          }
          else if(choice==5)
          {
              pair<int,int>to_user=users_manager.Read_user_id();
              if(to_user.first!=-1){
              questions_manager.ask_question(to_user,users_manager.current_user);
              questions_manager.Update_database();
              }
          }
          else if(choice==6)
          {
              users_manager.listusers_names_ids();
          }
          else if(choice==7)
            questions_manager.listfeed();
          else
            break;

		}
		run();
    }
};

int main()
{

    AskMeSystem services;
    services.run();


    return 0;
}
