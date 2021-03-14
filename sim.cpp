#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct labels{
    string label;
    int32_t line_number_of_label;
};

struct memory_elements{
    string name;
    int start_address;
};

class simulator{
    public:
    string registers[32]={"zero","at","v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6",
	"t7","s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","s8","ra"};
    int value_of_registers[32];
    vector<string> program;
    int number_of_instructions;
    string present_instruction;
    int memory_element_values[1000];
    void read_instruction(int line_number);
    int registers_in_present_instruction[3];
    string operations_allowed[17]={"add","sub","mul","and","or","nor","slt","addi","andi","ori","slti","lw",
	"sw","beq","bne","j","la"};
    int pc ;
    int position_of_element_in_memory;
    vector<struct labels>labels_in_program;
    vector<struct memory_elements>memory_used_in_program;
    void RemoveSpaces(string &str);
    void display();
    void run();
    void execute_present_operation(int n);  //******
    void valid_register(string s,int n);
    simulator(ifstream& file);

};
void simulator::run(){
    //value_of_registers[32]={0};
    for(int i=0;i<32;i++){
        value_of_registers[i]=0;
    }
    memory_element_values[1000]={0};
    position_of_element_in_memory=0;
   // value_of_registers[2]=10;
    int data_found=-1;
    int text_found=-1;
    for(int i=0;i<number_of_instructions;i++){
        string s=program[i];
        RemoveSpaces(s);
        if(s==".data"){
            data_found=i;
            break;
        }
    }
    if(data_found==-1){
        cout<<"error as .data is not found";
    }
    for(int j=data_found+1;j<number_of_instructions;j++){
        string s=program[j];
        RemoveSpaces(s);
        if(s==".text"){
            text_found=j;
            break;
        }
    }
    if(text_found==-1){
        cout<<"error as .text is not found";
    }

     for(int j=data_found+1;j<text_found;j++){ 
        int x=-1;
        string s = program[j];
    
        RemoveSpaces(s);
        
         x = s.find(":");
        
         string list_name;
         memory_elements m;
        
         if(x!=-1 && x!=0){
            list_name=s.substr(0,x);
            
            m.name=list_name;
            m.start_address=position_of_element_in_memory;
            memory_used_in_program.push_back(m);
            string s1=s.substr(x+6);
            stringstream commas(s1);
            string intermediate1;
            while(getline(commas,intermediate1,',')){
          
            memory_element_values[position_of_element_in_memory++]=stoi(intermediate1);
            }
        }
        for(int i=0;i<8;i++){
            cout<<memory_element_values[i]<<endl;
        }
     }   
    
    for(int k=text_found+1;k<number_of_instructions;k++){
        int x=-1;
        string s = program[k];
        RemoveSpaces(s);
         x = s.find(":");
        string label_name;
        labels l;
        if(x!=-1 && x!=0){
            label_name=s.substr(0,x);
            l.label=label_name;
            l.line_number_of_label=k;
            labels_in_program.push_back(l);
        }
        
    }
    pc=text_found+2;//extra
    for(int l=0;l<labels_in_program.size();l++){
        cout<<labels_in_program[l].label<<endl;
    }
   // cout<<pc<<"////////////////////////////////////////////////////"<<endl;
    cout<<"number of instructions"<<number_of_instructions<<endl; 
    while(pc<number_of_instructions){
       
        cout<<pc;
        string s2=program[pc];
        
        cout<<present_instruction<<"..............................."<<endl;
       
        RemoveSpaces(s2);
        if(s2==""){
            pc++;
            continue;
        }
        read_instruction(pc);
       // break;
    }
}
void simulator::RemoveSpaces(string &str)  
{ 
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    //return str; 
}
simulator::simulator(ifstream& file){
   
    number_of_instructions=0;
    pc=0;
    file.open("coo1.s");
    if(!file.is_open()){
        cout<<"unable to open file";
    }
    else{
        cout<<"opened file";
        while(file.good()){
            number_of_instructions++;
            string line;
            getline(file,line);
        cout<<line<<endl;
        program.push_back(line);
        }
    }
   
    file.close();  
}
void simulator::read_instruction(int line_number){
    present_instruction=program[line_number];
    cout<<present_instruction<<endl;
    pc=line_number;
    string operation;
    //RemoveSpaces(present_instruction);
    int is_label=-1;
    is_label=present_instruction.find(":");
    
    int count=0;
    int size_of_operation;
    for(int i=0;i<4;i++){
        if(present_instruction[i]==' '|| present_instruction[i]=='\t')
		{
			break;
		}
        count++;
    }
    operation =present_instruction.substr(0,count);
    cout<<operation<<"000"<<"1111111111111111111111111111111111111111111111111"<<endl;
    string registers_in_instruction;
    if(count<present_instruction.size()-1)
        registers_in_instruction=present_instruction.substr(count+1);
    RemoveSpaces(registers_in_instruction);    
    int position_in_array=-1;
    for(int i=0;i<17;i++){
        if(operation==operations_allowed[i]){
            position_in_array=i;
            break;
        }
    }
    if(is_label==-1){
    
    if(position_in_array==-1){
        cout<<"invalid-operation";
    }
    else if(position_in_array<7){
        registers_in_present_instruction[3]={0};
        valid_register(registers_in_instruction,6); //***
        execute_present_operation(position_in_array);  //****
    }
    else if(position_in_array<11){

        registers_in_present_instruction[3]={0};
        valid_register(registers_in_instruction,10);
        execute_present_operation(position_in_array);
    }
    else if(position_in_array<13){
        registers_in_present_instruction[3]={0};
        valid_register(registers_in_instruction,12);
        execute_present_operation(position_in_array);
        //**********  execute
    }
    else if(position_in_array<15){
        registers_in_present_instruction[3]={0};
        valid_register(registers_in_instruction,14);
        execute_present_operation(position_in_array);
    }
    else if(position_in_array==15){
        registers_in_present_instruction[3]={0};
        valid_register(registers_in_instruction,15);
        execute_present_operation(position_in_array);
    }
    else if(position_in_array==16){
        registers_in_present_instruction[3]={0};
        valid_register(registers_in_instruction,16);
        execute_present_operation(position_in_array);
    }
    }
    else{
        pc++;
    }

}
void simulator::execute_present_operation(int n){
    switch(n){
        case 0:
        if(registers_in_present_instruction[0]!=0 && registers_in_present_instruction[0]!=1 && registers_in_present_instruction[1]!=1 && registers_in_present_instruction[2]!=1){ 
		value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]]+value_of_registers[registers_in_present_instruction[2]]; 
        }
        pc++;
        break;
        case 1:
        if(registers_in_present_instruction[0]!=0 && registers_in_present_instruction[0]!=1 && registers_in_present_instruction[1]!=1 && registers_in_present_instruction[2]!=1){ 
		value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]]-value_of_registers[registers_in_present_instruction[2]]; 
        }
        pc++;
        break;
        case 2:
        if(registers_in_present_instruction[0]!=0 && registers_in_present_instruction[0]!=1 && registers_in_present_instruction[1]!=1 && registers_in_present_instruction[2]!=1){ 
		value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]]*value_of_registers[registers_in_present_instruction[2]]; 
        }
        pc++;
        break;
        case 3:
        if(registers_in_present_instruction[0]!=0 && registers_in_present_instruction[0]!=1 && registers_in_present_instruction[1]!=1 && registers_in_present_instruction[2]!=1){ 
		value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]] & value_of_registers[registers_in_present_instruction[2]]; 
        }
        pc++;
        break;
        case 4:
        if(registers_in_present_instruction[0]!=0 && registers_in_present_instruction[0]!=1 && registers_in_present_instruction[1]!=1 && registers_in_present_instruction[2]!=1){ 
		value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]]|value_of_registers[registers_in_present_instruction[2]]; 
        }
        pc++;
        break;
        case 5:

        break;
        case 6:
            value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]]<value_of_registers[registers_in_present_instruction[2]];
        break;
        case 7:
           
            cout<<registers_in_present_instruction[0]<<endl;
            cout<<registers_in_present_instruction[2]<<endl;
            value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2];
            pc++;
            break;
        case 8:
            cout<<registers_in_present_instruction[0]<<endl;
            cout<<registers_in_present_instruction[2]<<endl;
            value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]] & registers_in_present_instruction[2];
            pc++;
            break;
        break;
        case 9:
            cout<<registers_in_present_instruction[0]<<endl;
            cout<<registers_in_present_instruction[2]<<endl;
            value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]] | registers_in_present_instruction[2];
            pc++;
            break;
        break;
        case 10:

        break;
        case 11:
           
             cout<<registers_in_present_instruction[0]<<endl;
              cout<<registers_in_present_instruction[1]<<endl;
             cout<<registers_in_present_instruction[2]<<endl;
             value_of_registers[registers_in_present_instruction[0]]=memory_element_values[value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]/4];
             cout<<memory_element_values[value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]/4]<<"mmmmmmmmmmmmmmmm"<<endl;
            pc++;
        break;
        case 12:
            
             cout<<registers_in_present_instruction[0]<<endl;
              cout<<registers_in_present_instruction[1]<<endl;
             cout<<registers_in_present_instruction[2]<<endl;
             memory_element_values[value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]/4]=value_of_registers[registers_in_present_instruction[0]];
            
            pc++; 
        break;
        case 13:
            if(value_of_registers[registers_in_present_instruction[0]]==value_of_registers[registers_in_present_instruction[1]]){
                pc=registers_in_present_instruction[2]+1;
            }
            else{
                pc++;
            }
        break;
        case 14:
            if(value_of_registers[registers_in_present_instruction[0]]!=value_of_registers[registers_in_present_instruction[1]]){
                pc=registers_in_present_instruction[2]+1;
            }
            else{
                pc++;
            }
        break;
        case 15:
            pc = registers_in_present_instruction[0];
        break;
        case 16:
            cout<<registers_in_present_instruction[0]<<endl;
            cout<<registers_in_present_instruction[1]<<endl;
            value_of_registers[registers_in_present_instruction[0]]=registers_in_present_instruction[1];
            pc++;
        break;
        case 17:

        break;
    }
}
//void simulator::add
void simulator::valid_register(string s,int n){
    vector <string> tokens;
    stringstream commas(s);
    string intermediate;
    while(getline(commas,intermediate,',')){
        if(intermediate[0]=='$')
        tokens.push_back(intermediate.substr(1));
        else
        tokens.push_back(intermediate);
    }
    int x=tokens.size();
    cout<<x<<endl;
    for(int i=0;i<tokens.size();i++){
        cout<<tokens[i]<<endl;
    }
    int y=0;
    if(n==6 && x==3){
            for(int i=0;i<x;i++){
                for(int j=0;j<32;j++){
                    if(tokens[i]==registers[j]){
                        registers_in_present_instruction[i]=j;
                        y++;
                    }
                }
            }
            if(y!=x){
                cout<<"all registers given are not valid";
            }  
    }
    else if(n==10 && x==3){
       // cout<<"tttttttttttttttttttttttttttttttttttttttttttttt"<<endl;
        for(int i=0;i<2;i++){
            for(int j=0;j<32;j++){
                if(tokens[i]==registers[j]){
                    
                    registers_in_present_instruction[i]=j;
                    cout<<registers_in_present_instruction[i]<<"jjjjjjjjjjjjjjjjjjj"<<endl;
                    y++;
                }
            }
        }
        cout<<tokens[2]<<endl;
        cout<<stoi(tokens[2])<<endl;
        registers_in_present_instruction[2]=stoi(tokens[2]);
        y++;
       
        if(y!=3){
            cout<<"all registers given are not valid";
        }
    }
    
    else if(n==12 && x==2){
         string s1=tokens[1];
        int offset=-1;
        int x=s1.find("(");
        offset=stoi(s1.substr(0,x));

        int z=s1.find(")");
        string s2 = s1.substr(x+2,z-x-2);
        for(int i=0;i<32;i++){
            if(registers[i]==tokens[0]){
                registers_in_present_instruction[0]=i;
                y++;
            }
        }
        for(int i=0;i<32;i++){
            if(registers[i]==s2){
                registers_in_present_instruction[1]=i;
                y++;
            }
        }
        if(offset==-1){
            cout<<"wrong offset value";
        }
        registers_in_present_instruction[2]=offset;
        cout<<registers_in_present_instruction[0]<<endl;
        cout<<registers_in_present_instruction[1]<<endl;
        cout<<registers_in_present_instruction[2]<<endl;
      //  cout<<tokens[0]<<"||||||||||||||||||||||||||||||||||||"<<endl;
      //  cout<<s2<<"++++++++++++++++++++++++++++++++++++"<<endl;
      
        if(y!=2){
        cout<<"wrong"<<endl;
        }
   
    }
    else if(n==14 && x==3){
        for(int i=0;i<2;i++){
            for(int j=0;j<32;j++){
                if(tokens[i]==registers[j]){
                    registers_in_present_instruction[i]=j;
                    y++;
                }
            }
        }
        if(y!=2){
            cout<<"error in number of registers,beq,bne";
        }
        int found=0;
        for(int k=0;k<labels_in_program.size();k++){
            if(tokens[2]==labels_in_program[k].label){
                registers_in_present_instruction[2]=labels_in_program[k].line_number_of_label;
                found=1;
            }
        }
        if(found==0){
            cout<<"wrong label name written";
        }

    }
    else if(n==15){
        string s=tokens[0];
        int found=0;
        for(int i=0;i<labels_in_program.size();i++){
            if(s==labels_in_program[i].label){
                registers_in_present_instruction[0]=labels_in_program[i].line_number_of_label;
                found=1;
            }
        }
        if(found==0){
            cout<<"wrong label name is written";
        }
    }
    else if(n==16){
    // cout<<"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"<<endl;
        for(int j=0;j<32;j++){
            if(tokens[0]==registers[j]){
                    registers_in_present_instruction[0]=j;
                    y++;
             }
         }
        for(int j=0;j<memory_used_in_program.size();j++){
            if(tokens[1]==memory_used_in_program[j].name){
                registers_in_present_instruction[1]=memory_used_in_program[j].start_address;
            }
        }

        registers_in_present_instruction[2]==-1000;

        if(y!=1){
            cout<<"error in number of registers,beq,bne";
        }
    }
    else{
            cout<<"wrong number of instructions"<<" in valid_register";
        }

}
void simulator::display(){
     for(int i=0;i<32;i++){
         cout<<registers[i]<<"  "<<value_of_registers[i]<<endl;
     }
}

int main(){
     ifstream file;
     simulator s(file);
     s.run();
     s.display();
}