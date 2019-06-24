#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;


struct AlphaVals{
    string vlan;
    string ip_add_IN;
    string ip_add_R1;
    string ip_add_R2;
    string ip_add_GW;
    string ip_add_NET;
    string ip_add_MASK;
    string account_no;
    string company_name;
    int bandwidth;
};

void GenConf(string *vlan, string *ip_add, string *account_no, string *company_name, int *bandwidth, string *ipadd_gw, string *ipadd_net, string *ipadd_in, string *ipadd_MASK, int pri){
    replace(company_name->begin(), company_name->end(), ' ', '_');
    ofstream file;
    if(pri == 150){
        file.open("Router1.txt");
        file << "=========Router 1============" << endl;
    }
    else{
        file.open("Router2.txt");
        file << "=========Router 2============" << endl;
    }

 	file << "=========Copy paste Below============" << endl;
    file << "";
    file << "interface Vlan" << *vlan << endl;
    file << " description " << *account_no << *company_name << endl;
    file << " ip address " << *ip_add +"/"+*ipadd_MASK<< endl;
    file << " ip router ospf 100 area 0.0.0.1"<< endl;
    file << " hsrp version 2"<< endl;
    file << "  hsrp " << *vlan << endl;
    file << "  preempt"<< endl;
    file << "  priority "<< pri << endl;
    file << "  ip " << *ipadd_gw << endl;
    file << endl;
    file << ""<< endl;
    file << "ip access-list ACL_" << *company_name <<"#"<< *account_no <<"M"<< *bandwidth<< endl;
    file << " 10 permit ip any " << *ipadd_in << endl;
    file << " 20 permit ip " << *ipadd_in <<" any"<< endl;
    file << endl;
    file << ""<< endl;
    file << "class-map type qos match-all " << *company_name <<"#"<< *account_no <<"M"<< *bandwidth<< endl;
    file << " match access-group name ACL_"<< *company_name <<"#"<< *account_no <<"M"<< *bandwidth<< endl;
    file << endl;
    file << ""<< endl;
    file << "policy-map type qos "<< *company_name <<"#"<< *account_no <<"M"<< *bandwidth<< endl;
    file << " class "<< *company_name <<"#"<< *account_no <<"M"<< *bandwidth<< endl;
    file << "  police cir "<< *bandwidth << " mbps bc 200 ms conform transmit violate drop" << endl;
    file << endl;
    file << ""<< endl;
    file << "vlan configuration " << *vlan << endl;
    file << " service-policy type qos output "<< *company_name <<"#"<< *account_no <<"M"<< *bandwidth<< endl;
    file << " service-policy type qos input "<< *company_name <<"#"<< *account_no <<"M"<< *bandwidth<< endl;
    file << endl;
    file << ""<< endl;
    file << "spanning-tree vlan "<< *vlan << " priority 4096" << endl;
    file << "============Until Here================" << endl << endl << endl;

    file.close();
}


void IPaddExt(string *ipaddIN, string *ipaddR1, string *ipaddR2, string *ipaddGW, string *ipaddNET, string *ipaddMASK){

    string ip_oct[4];
    string octR1, octR2, octGW, octNET;
    string ipadd3oct;

    int ip_oct_int[4];
    int ip_mask_int;

    char delim = '.';
    char slash = '/';

    int x = 0;
    int dotloc = 0;
    int slashloc = 0;

/*SPLIT IP ADDRESS OCTET AND STORE IN IP_OCT[]*/
    for(int i = 0; i < 4; i++){
        dotloc = ipaddIN->find(delim,dotloc);
        ip_oct[i] = ipaddIN->substr(x,dotloc-x);
        x = dotloc + 1;
        dotloc++;
    }

/*EXTRACT NETMASK FROM LAST OCTET*/
        slashloc = ip_oct[3].find(slash,slashloc);
        *ipaddMASK = ip_oct[3].substr(slashloc+1,2);
        ip_oct[3] = ip_oct[3].substr(0,slashloc);


/*CONVERT OCTET/MASK FROM STRING TO INT*/
    for(int i = 0; i < 4; i++){
        ip_oct_int[i] = stoi(ip_oct[i]);
    }
        //ip_mask_int = stoi(*ipaddMASK);


/*CONVERT LAST OCTET TO STRING AND CORRES FUNC*/
    octNET = to_string(ip_oct_int[3]);
    octGW = to_string(ip_oct_int[3]+1);
    octR1 = to_string(ip_oct_int[3]+2);
    octR2 = to_string(ip_oct_int[3]+3);


/*APPEND THE CORRES OCT*/
    ipadd3oct.append(ip_oct[0]).append(".").append(ip_oct[1]).append(".").append(ip_oct[2]).append(".");

    *ipaddR1 = ipadd3oct + octR1;
    *ipaddR2 = ipadd3oct + octR2;
    *ipaddGW = ipadd3oct + octGW;
    *ipaddNET = ipadd3oct + octNET;

}


int main(){

    AlphaVals Router;
    AlphaVals *pRouter = &Router;

    cout << "VLAN: "; getline (cin, pRouter->vlan);
    cout << "IP Address: "; getline (cin, pRouter->ip_add_IN);
    cout << "Acct Nos: "; getline (cin, pRouter->account_no);
    cout << "Company Name: "; getline (cin, pRouter->company_name);
    cout << "Bandwidth: ";cin >> pRouter->bandwidth;
    cout <<""<<endl;


    IPaddExt(&pRouter->ip_add_IN, &pRouter->ip_add_R1, &pRouter->ip_add_R2, &pRouter->ip_add_GW, &pRouter->ip_add_NET, &pRouter->ip_add_MASK);

    GenConf(&pRouter->vlan, &pRouter->ip_add_R1, &pRouter->account_no, &pRouter->company_name, &pRouter->bandwidth, &pRouter->ip_add_GW, &pRouter->ip_add_NET, &pRouter->ip_add_IN, &pRouter->ip_add_MASK, 150);
    GenConf(&pRouter->vlan, &pRouter->ip_add_R2, &pRouter->account_no, &pRouter->company_name, &pRouter->bandwidth, &pRouter->ip_add_GW, &pRouter->ip_add_NET, &pRouter->ip_add_IN, &pRouter->ip_add_MASK, 120);

    return 0;
}
