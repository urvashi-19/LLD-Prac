#include <bits/stdc++.h>
using namespace std;

// Forward Declaration

class User;
class Profile;
class Question;
class Answer;
class Image;

//==================== IMAGE ====================//

class Image{

private:

    int imageId;
    string url;

public:

    Image(int imageId,string url){
        this->imageId = imageId;
        this->url = url;
    }

    int getImageId(){
        return imageId;
    }

    string getUrl(){
        return url;
    }
};

//==================== PROFILE ====================//

class Profile{

private:

    vector<Question*> questions;
    vector<Answer*> answers;

public:

    void addQuestion(Question* q){
        questions.push_back(q);
    }

    void addAnswer(Answer* a){
        answers.push_back(a);
    }

    vector<Question*> getQuestions(){
        return questions;
    }

    vector<Answer*> getAnswers(){
        return answers;
    }

};

//==================== USER ====================//

class User{

private:

    int userId;
    string name;
    Profile* profile;

public:

    User(int userId,
         string name){

        this->userId = userId;
        this->name = name;
        profile = new Profile();
    }

    int getUserId(){
        return userId;
    }

    string getName(){
        return name;
    }

    Profile* getProfile(){
        return profile;
    }

};

//==================== BASE POST ====================//

class Post{

protected:

    int postId;
    string description;

    User* author;

    vector<Image*> images;

    int upVotes;
    int downVotes;

public:

    Post(int postId,
         string description,
         User* author){

        this->postId = postId;
        this->description = description;
        this->author = author;

        upVotes = 0;
        downVotes = 0;
    }

    virtual ~Post(){}

    int getPostId(){
        return postId;
    }

    string getDescription(){
        return description;
    }

    User* getAuthor(){
        return author;
    }

    void addImage(Image* image){
        images.push_back(image);
    }

    vector<Image*> getImages(){
        return images;
    }

    void upVote(){
        upVotes++;
    }

    void downVote(){
        downVotes++;
    }

    int getUpVotes(){
        return upVotes;
    }

    int getDownVotes(){
        return downVotes;
    }

};

//==================== ANSWER ====================//

class Answer : public Post{

public:

    Answer(int answerId,
           string description,
           User* author)
    : Post(answerId,
           description,
           author)
    {

    }

};

//==================== QUESTION ====================//

class Question : public Post{

private:

    string title;

    vector<Answer*> answers;

public:

    Question(int questionId,
             string title,
             string description,
             User* author)

    : Post(questionId,
           description,
           author){

        this->title = title;
    }

    string getTitle(){
        return title;
    }

    void addAnswer(Answer* answer){
        answers.push_back(answer);
    }

    vector<Answer*> getAnswers(){
        return answers;
    }

};



//==================== STACK OVERFLOW ====================//

class StackOverflow{

private:

    vector<Question*> questions;

public:

    // Post Question

    void postQuestion(Question* question){

        questions.push_back(question);

        question->getAuthor()
                ->getProfile()
                ->addQuestion(question);

        cout<<"Question Posted Successfully\n";
    }

    // Post Answer

    void postAnswer(Question* question,
                    Answer* answer){

        question->addAnswer(answer);

        answer->getAuthor()
              ->getProfile()
              ->addAnswer(answer);

        cout<<"Answer Posted Successfully\n";
    }

    // Search Question

    vector<Question*> searchQuestion(string keyword){

        vector<Question*> result;

        for(Question* question : questions){

            if(question->getTitle().find(keyword)
               != string::npos){

                result.push_back(question);
            }
        }

        return result;
    }

    // Upload Image

    void uploadImage(Post* post,
                     Image* image){

        post->addImage(image);

        cout<<"Image Uploaded Successfully\n";
    }

    // Upvote

    void upVote(Post* post){

        post->upVote();

        cout<<"Post Upvoted\n";
    }

    // Downvote

    void downVote(Post* post){

        post->downVote();

        cout<<"Post Downvoted\n";
    }

    // Display Question

    void displayQuestion(Question* question){

        cout<<"\n--------------------------\n";

        cout<<"Title : "
            <<question->getTitle()
            <<endl;

        cout<<"Description : "
            <<question->getDescription()
            <<endl;

        cout<<"Posted By : "
            <<question->getAuthor()->getName()
            <<endl;

        cout<<"UpVotes : "
            <<question->getUpVotes()
            <<endl;

        cout<<"DownVotes : "
            <<question->getDownVotes()
            <<endl;

        cout<<"Answers : "
            <<question->getAnswers().size()
            <<endl;

        cout<<"--------------------------\n";
    }

    // Display Answers

    void displayAnswers(Question* question){

        cout<<"\nAnswers\n\n";

        for(Answer* answer : question->getAnswers()){

            cout<<"Author : "
                <<answer->getAuthor()->getName()
                <<endl;

            cout<<"Answer : "
                <<answer->getDescription()
                <<endl;

            cout<<"UpVotes : "
                <<answer->getUpVotes()
                <<endl;

            cout<<"DownVotes : "
                <<answer->getDownVotes()
                <<endl;

            cout<<"-----------------\n";
        }

    }

};

int main() {

    // Create StackOverflow System

    StackOverflow stackOverflow;

    // Create Users

    User* user1 = new User(1, "Rahul");
    User* user2 = new User(2, "Aman");

    // Create Question

    Question* question = new Question(
        101,
        "What is Polymorphism?",
        "Can anyone explain runtime polymorphism with an example?",
        user1
    );

    // Post Question

    stackOverflow.postQuestion(question);

    // Create Answer

    Answer* answer = new Answer(
        201,
        "Runtime polymorphism is achieved using virtual functions.",
        user2
    );

    // Post Answer

    stackOverflow.postAnswer(question, answer);

    // Upload Image

    Image* image = new Image(
        1,
        "https://stackoverflow.com/image1.png"
    );

    stackOverflow.uploadImage(question, image);

    // Upvote Question

    stackOverflow.upVote(question);
    stackOverflow.upVote(question);

    // Upvote Answer

    stackOverflow.upVote(answer);

    // Downvote Answer

    stackOverflow.downVote(answer);

    // Search Question

    cout << "\nSearch Result\n";

    vector<Question*> result =
        stackOverflow.searchQuestion("Polymorphism");

    for(Question* q : result){

        cout << q->getTitle() << endl;

    }

    // Display Question

    stackOverflow.displayQuestion(question);

    // Display Answers

    stackOverflow.displayAnswers(question);

    // Cleanup

    delete image;
    delete answer;
    delete question;
    delete user1;
    delete user2;

    return 0;
}