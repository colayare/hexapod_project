#include <math.h>   //CHECK IF NEEDED
#include <cmath>
#include "globals/global_defines.h"
#include "hexapod/hexapod_kinematics.h"
#include "hexapod/hexapod_locomotion.h"
#include "params/hexapod_ip_params.h"
#include "params/hexapod_params.h"

uint32_t hexapod_locomotion::iteration_size() {
    return this->S/this->res + 1;
}

uint32_t hexapod_locomotion::step(float step_idx, uint32_t walk, float alpha) {
    this->get_step(step_idx, walk, alpha);
    
    this->write_ik_input(this->ef_position.leg[0]);
    this->write_ik_input(this->ef_position.leg[1]);
    this->write_ik_input(this->ef_position.leg[2]);
    this->write_ik_input(this->ef_position.leg[3]);
    this->write_ik_input(this->ef_position.leg[4]);
    this->write_ik_input(this->ef_position.leg[5]);
    this->write_fifo();
    this->trigger_ikinematics();
    
    return 1;
}

uint32_t hexapod_locomotion::get_step(float step_idx, uint32_t walk, float alpha) {
    ef_pos_t ef;
    
    if ( ! (this->S && this->res && this-> xo && this->yo && this->zo) ) {
        cout << "ERROR: Locomotion parameters no initialized." << endl;
        return 0;
    }
    
    float i = this->res * step_idx;
    
    switch (walk) {
        case 1:
            // First Stage: 0 < i < S/2
            if ( i <= this->S/2 ) {
                ef.leg[0].x = this->xo + i*sin(-alpha);
                ef.leg[0].y = this->yo + i*cos(-alpha);
                ef.leg[1].x = this->xo + (this->S/2 - i) * sin(alpha);
                ef.leg[1].y = this->yo + (this->S/2 - i) * cos(alpha);
                ef.leg[1].z = this->zo;
                ef.leg[2].x = this->xo + (this->S/4 - i) * sin(-alpha);
                ef.leg[2].y = 0 + (this->S/4 - i) * cos(-alpha);
                ef.leg[2].z = this->zo;
                ef.leg[3].x = this->xo + (i - this->S/4) * sin(-alpha);
                ef.leg[3].y = 0 + (i - this->S/4) * cos(-alpha);
                ef.leg[4].x = this->xo + i * sin(-alpha);
                ef.leg[4].y = -this->yo + i * cos(-alpha);
                ef.leg[5].x = this->xo + (this->S/2 - i) * sin(alpha);
                ef.leg[5].y = -this->yo + (this->S/2 - i) * cos(alpha);
                ef.leg[5].z = this->zo;
            } 
            if ( this-> S != 0 ) {
                ef.leg[0].z = -(8 * pow(i - this->S/4, 2)/this->S - this->zo - this->S/2);
                ef.leg[3].z = -(8 * pow(i - this->S/4, 2)/this->S - this->zo - this->S/2);
                ef.leg[4].z = -(8 * pow(i - this->S/4, 2)/this->S - this->zo - this->S/2);
                
            } else {
                ef.leg[0].z = this->zo + this->S/2;
                ef.leg[3].z = this->zo + this->S/2;
                ef.leg[4].z = this->zo;
            }
            // Second Stage: S/2 < i <= S
            if ( i > this->S/2 && i <= this->S) {
                ef.leg[0].x = this->xo + (this->S - i) * sin(-alpha);
                ef.leg[0].y = this->yo + (this->S - i) * cos(-alpha);
                ef.leg[0].z = this->zo;
                ef.leg[1].x = this->xo + (i - this->S/2) * sin(alpha);
                ef.leg[1].y = this->yo + (i - this->S/2) * cos(alpha);
                ef.leg[1].z = -(8 * pow(i - 3*this->S/4, 2)/this->S - this->zo - this->S/2);
                ef.leg[2].x = this->xo + (i - 3*this->S/4) * sin(-alpha);
                ef.leg[2].y = 0 + (i - 3*this->S/4) * cos(-alpha);
                ef.leg[2].z = -(8 * pow(i - 3*this->S/4, 2)/this->S - this->zo - this->S/2);
                ef.leg[3].x = this->xo + (3*this->S/4 - i) * sin(-alpha);
                ef.leg[3].y = 0 + (3*this->S/4 - i) * cos(-alpha);
                ef.leg[3].z = this->zo;
                ef.leg[4].x = this->xo - (i - this->S) * sin(-alpha);
                ef.leg[4].y = -this->yo - (i - this->S) * cos(-alpha);
                ef.leg[4].z = this->zo;
                ef.leg[5].x = this->xo + (i - this->S/2) * sin(alpha);
                ef.leg[5].y = -this->yo + (i - this->S/2) * cos(alpha);
                ef.leg[5].z = -(8 * pow(i - 3*this->S/4, 2)/this->S - this->zo - this->S/2);
            }
            break;
        case 2:
            // First Stage: 0 < i <= S/3
            if ( i <= this->S/3 ) {
                ef.leg[0].x = this->xo + 1.5*i*sin(-alpha);
                ef.leg[0].y = this->yo + 1.5*i*cos(-alpha);
                ef.leg[1].x = this->xo - 0.75*i*sin(alpha);
                ef.leg[1].y = this->yo - 0.75*i*cos(alpha);
                ef.leg[1].z = this->zo;
                ef.leg[2].x = this->xo - 0.75*i*sin(-alpha);
                ef.leg[2].y = 0 - 0.75*i*cos(-alpha);
                ef.leg[2].z = this->zo;
                ef.leg[3].x = this->xo + 1.5*i*sin(alpha);
                ef.leg[3].y = 0 + 1.5*i*cos(alpha);
                ef.leg[4].x = this->xo - 0.75*i*sin(-alpha);
                ef.leg[4].y = -this->yo - 0.75*i*cos(-alpha);
                ef.leg[4].z = this->zo;
                ef.leg[5].x = this->xo - 0.75*i*sin(alpha);
                ef.leg[5].y = -this->yo - 0.75*i*cos(alpha);
                ef.leg[5].z = this->zo;
            }
            if ( this->S != 0 ) {
                ef.leg[0].z = -(8 * pow(1.5*i - this->S/4, 2)/this->S - this->zo - this->S/2);
                ef.leg[3].z = -(8 * pow(1.5*i - this->S/4, 2)/this->S - this->zo - this->S/2);
            } else {
                ef.leg[0].z = this->zo + this->S/2;
                ef.leg[3].z = this->zo + this->S/2;
            }
            // Second Stage: S/3 < i <= 2*S/3
            if ( i > this->S/3 && i <= 2*this->S/3 ) {
                ef.leg[0].x = this->xo + 0.75*(this->S - i)*sin(-alpha);
                ef.leg[0].y = this->yo + 0.75*(this->S - i)*cos(-alpha);
                ef.leg[0].z = this->zo;
                ef.leg[1].x = this->xo - 0.75*i*sin(alpha);
                ef.leg[1].y = this->yo - 0.75*i*cos(alpha);
                ef.leg[1].z = this->zo;
                ef.leg[2].x = this->xo + (1.5*i - 0.75*this->S)*sin(-alpha);
                ef.leg[2].y = 0 + (1.5*i - 0.75*this->S)*cos(-alpha);
                ef.leg[2].z = -(8 * pow(1.5*i - 0.75*this->S, 2)/this->S - this->zo - this->S/2);
                ef.leg[3].x = this->xo+0.75*(this->S-i)*sin(alpha);
                ef.leg[3].y = 0 + 0.75*(this->S - i)*cos(alpha);
                ef.leg[3].z = this->zo;
                ef.leg[4].x = this->xo - 0.75*i*sin(-alpha);
                ef.leg[4].y = -this->yo - 0.75*i*cos(-alpha);
                ef.leg[4].z = this->zo;
                ef.leg[5].x = this->xo + (1.5*i - 0.75*this->S)*sin(alpha);
                ef.leg[5].y = -this->yo + (1.5*i - 0.75*this->S)*cos(alpha);
                ef.leg[5].z = -(8 * pow(1.5*i - 0.75*this->S, 2)/this->S - this->zo - this->S/2);
            }
            // Third Stage: 2*S/3 < i <= S
            if ( i > 2*this->S/3 && i <= this->S ) {
                ef.leg[0].x = this->xo + 0.75*(this->S - i)*sin(-alpha);
                ef.leg[0].y = this->yo + 0.75*(this->S - i)*cos(-alpha);
                ef.leg[0].z = this->zo;
                ef.leg[1].x = this->xo + 1.5*(i - this->S)*sin(alpha);
                ef.leg[1].y = this->yo + 1.5*(i - this->S)*cos(alpha);
                ef.leg[1].z = -(8 * pow(1.5*(i - this->S) + this->S/4, 2)/this->S - this->zo - this->S/2);
                ef.leg[2].x = this->xo + 0.75*(this->S - i)*sin(-alpha);
                ef.leg[2].y = 0 + 0.75*(this->S - i)*cos(-alpha);
                ef.leg[2].z = this->zo;
                ef.leg[3].x = this->xo + 0.75*(this->S - i)*sin(alpha);
                ef.leg[3].y = 0 + 0.75*(this->S - i)*cos(alpha);
                ef.leg[3].z = this->zo;
                ef.leg[4].x = this->xo + 1.5*(i - this->S)*sin(-alpha);
                ef.leg[4].y = -this->yo + 1.5*(i - this->S)*cos(-alpha);
                ef.leg[4].z = -(8 * pow(1.5*(i - this->S) + this->S/4, 2)/this->S - this->zo - this->S/2);
                ef.leg[5].x = this->xo + 0.75*(this->S - i)*sin(alpha);
                ef.leg[5].y = -this->yo + 0.75*(this->S - i)*cos(alpha);
                ef.leg[5].z = this->zo;
            }
            break;
        case 3:
            // First Stage: 0 < i <= S/2
            if ( i <= this->S/4 ) {
                ef.leg[0].x = this->xo + 2*i*sin(-alpha);
                ef.leg[0].y = this->yo + 2*i*cos(-alpha);
                ef.leg[2].x = this->xo - 2*i/3*sin(-alpha);
                ef.leg[2].y = 0 - 2*i/3*cos(-alpha);
                ef.leg[2].z = this->zo;
                ef.leg[3].x = this->xo - 2*i/3*sin(alpha);
                ef.leg[3].y = 0 - 2*i/3*cos(alpha);         
                ef.leg[3].z = this->zo;
                ef.leg[5].x = this->xo + 2*i*sin(alpha);     
                ef.leg[5].y = -this->yo + 2*i*cos(alpha);
            }
            if ( this->S != 0 ) {
                ef.leg[0].z = -(8 * pow(2*i - this->S/4, 2)/this->S - this->zo - this->S/2);
                ef.leg[5].z = -(8 * pow(2*i - this->S/4, 2)/this->S - this->zo - this->S/2);
            } else {
                ef.leg[0].z = this->zo + this->S/2;
                ef.leg[5].z = this->zo + this->S/2;
            }
            if ( i <= this->S/2 ) {
                ef.leg[1].x = this->xo - 2*i/3*sin(alpha);
                ef.leg[1].y = this->yo - 2*i/3*cos(alpha);
                ef.leg[1].z = this->zo;
                ef.leg[4].x = this->xo - 2*i/3*sin(-alpha);
                ef.leg[4].y = -this->yo - 2*i/3*cos(-alpha);
                ef.leg[4].z = this->zo;
            }
            
            // Second Stage: S/4 < i <= 3*S/4
            if ( i > this->S/4 && i <= 3*this->S/4 ) {
                ef.leg[0].x = this->xo + 2*(this->S - i)/3*sin(-alpha);
                ef.leg[0].y = this->yo + 2*(this->S - i)/3*cos(-alpha);
                ef.leg[0].z = this->zo;
                ef.leg[2].x = this->xo - 2*i/3*sin(-alpha);        
                ef.leg[2].y = 0 - 2*i/3*cos(-alpha);           
                ef.leg[2].z = this->zo;
                ef.leg[5].x = this->xo + 2*(this->S - i)/3*sin(alpha);       
                ef.leg[5].y = -this->yo + 2*(this->S - i)/3*cos(alpha);      
                ef.leg[5].z = this->zo;
            }
            if ( i > this->S/2 && i <= 3*this->S/4) {
                ef.leg[1].x = this->xo + (2*i - 4*this->S/3)*sin(alpha);     
                ef.leg[1].y = this->yo + (2*i - 4*this->S/3)*cos(alpha);     
                ef.leg[1].z = -(8 * pow(2*i - 4*this->S/3 + this->S/12, 2)/this->S - this->zo - this->S/2);
                ef.leg[4].x = this->xo + (2*i - 4*this->S/3)*sin(-alpha);    
                ef.leg[4].y = -this->yo + (2*i - 4*this->S/3)*cos(-alpha);   
                ef.leg[4].z = -(8*pow(2*i - 4*this->S/3 + this->S/12, 2)/this->S - this->zo - this->S/2);
            }
            if ( i > this->S/4 && i <= this->S/2 ) {
                ef.leg[3].x = this->xo + 2*(i - this->S/3)*sin(alpha);       
                ef.leg[3].y = 0 + 2*(i - this->S/3)*cos(alpha);        
                ef.leg[3].z = -(8 * pow(2*(i - this->S/3) - this->S/12, 2)/this->S - this->zo - this->S/2); 
            }                
            
            // Third Stage: S/2 < i <= S
            if ( i > 3*this->S/4 && i <= this->S ) {
                ef.leg[0].x = this->xo + 2*(this->S - i)/3*sin(-alpha);      
                ef.leg[0].y = this->yo + 2*(this->S - i)/3*cos(-alpha);      
                ef.leg[0].z = this->zo;
                ef.leg[1].x = this->xo + 2*(this->S - i)/3*sin(alpha);       
                ef.leg[1].y = this->yo + 2*(this->S - i)/3*cos(alpha);       
                ef.leg[1].z = this->zo;
                ef.leg[2].x = this->xo + 2*(i - this->S)*sin(-alpha);        
                ef.leg[2].y = 0 + 2*(i - this->S)*cos(-alpha);         
                ef.leg[2].z = -(8 * pow(2*(i - this->S)+this->S/4, 2)/this->S - this->zo - this->S/2);
                ef.leg[4].x = this->xo + 2*(this->S - i)/3*sin(-alpha);      
                ef.leg[4].y = -this->yo + 2*(this->S - i)/3*cos(-alpha);     
                ef.leg[4].z = this->zo;
                ef.leg[5].x = this->xo + 2*(this->S - i)/3*sin(alpha);       
                ef.leg[5].y = -this->yo + 2*(this->S - i)/3*cos(alpha);      
                ef.leg[5].z = this->zo;
            }
            if ( i > this->S/2 && i <= this->S ) {
                ef.leg[3].x = this->xo + 2*(this->S - i)/3*sin(alpha);       
                ef.leg[3].y = 0 + 2*(this->S - i)/3*cos(alpha);        
                ef.leg[3].z = this->zo;
            }
            break;
        case 4:
            // First Stage: 0 < i <= S/3
            if ( i <= this->S/3) {
                ef.leg[0].x = this->xo - 0.6*i*sin(-alpha);          
                ef.leg[0].y = this->yo - 0.6*i*cos(-alpha);          
                ef.leg[0].z = this->zo;
            }
            // First Stage: 0 < i <= 5*S/6
            if ( i <= 5*this->S/6 ) {
                ef.leg[1].x = this->xo - 0.6*i*sin(alpha);           
                ef.leg[1].y = this->yo - 0.6*i*cos(alpha);           
                ef.leg[1].z = this->zo;
            }
            // First Stage: 0 < i <= S/6
            if ( i<= this->S/6 ) {
                ef.leg[2].x = this->xo - 0.6*i*sin(-alpha);          
                ef.leg[2].y = 0 - 0.6*i*cos(-alpha);           
                ef.leg[2].z = this->zo;
            }
            // First Stage: 0 < i <= 2*S/3
            if ( i <= 2*this->S/3 ) {
                ef.leg[3].x = this->xo - 0.6*i*sin(alpha);           
                ef.leg[3].y = 0 - 0.6*i*cos(alpha);            
                ef.leg[3].z = this->zo;
            }
            // First Stage: 0 < i <= S/6
            if ( i <= this->S/6 ) {
                ef.leg[4].x = this->xo + 3*i*sin(-alpha);            
                ef.leg[4].y = -this->yo + 3*i*cos(-alpha);  
            }
            if ( this->S != 0 ) {
                  ef.leg[4].z = -(8 * pow(3*i - this->S/4, 2)/this->S - this->zo - this->S/2);
            } else {
                  ef.leg[4].z = this->zo + this->S/2;
            }
            if ( i <= this->S/2 ) {
                ef.leg[5].x = this->xo - 0.6*i*sin(alpha);           
                ef.leg[5].y = -this->yo - 0.6*i*cos(alpha);          
                ef.leg[5].z = this->zo;
            }
            
            // Second Stage: S/3 < i <= S/2
            if ( i > this->S/3 && i <= this->S/2 ) {
                ef.leg[0].x = this->xo + (3*i - 1.2*this->S)*sin(-alpha);    
                ef.leg[0].y = this->yo + (3*i - 1.2*this->S)*cos(-alpha);    
                ef.leg[0].z = -(8 * pow((3*i - 1.2*this->S) - this->S/20, 2)/this->S - this->zo - this->S/2);
            }
            // Second Stage: 5*S/6 < i <= S
            if ( i > 5*this->S/6 && i <= this->S ) {
                ef.leg[1].x = this->xo + 3*(i - this->S)*sin(alpha);         
                ef.leg[1].y = this->yo + 3*(i - this->S)*cos(alpha);         
                ef.leg[1].z = -(8 * pow(3*(i - this->S) + this->S/4, 2)/this->S - this->zo - this->S/2);
            }
            // Second Stage: S/6 < i <= S/3
            if ( i > this->S/6 &&  i <= this->S/3) {
                ef.leg[2].x = this->xo + (3*i - 0.6*this->S)*sin(-alpha);    
                ef.leg[2].y = 0 + (3*i - 0.6*this->S)*cos(-alpha);     
                ef.leg[2].z = -(8 * pow((3*i - 0.6*this->S) - 3*this->S/20, 2)/this->S - this->zo - this->S/2);
            }
            if ( i > 2*this->S/3 && i <= 5*this->S/6 ) {
                ef.leg[3].x = this->xo + (3*i - 2.4*this->S)*sin(alpha);     
                ef.leg[3].y = 0 + (3*i - 2.4*this->S)*cos(alpha);      
                ef.leg[3].z = -(8 * pow((3*i - 2.4*this->S) + 3*this->S/20, 2)/this->S - this->zo - this->S/2);
            }
            // Second Stage: S/6 < i <= S
            if ( i > this->S/6 && i <= this->S ) {
                ef.leg[4].x = this->xo + 0.6*(this->S - i)*sin(-alpha);      
                ef.leg[4].y = -this->yo + 0.6*(this->S - i)*cos(-alpha);     
                ef.leg[4].z = this->zo;
            }
            // Second Stage: S/2 < i <= 2*S/3
            if ( i > this->S/2 && i <= 2*this->S/3 ) {
                ef.leg[5].x = this->xo + (3*i - 1.8*this->S)*sin(alpha);     
                ef.leg[5].y = -this->yo + (3*i - 1.8*this->S)*cos(alpha);    
                ef.leg[5].z = -(8 * pow((3*i - 1.8*this->S) + this->S/20, 2)/this->S - this->zo - this->S/2);
            }
    
            // Third Stage: S/2 < i <= S
            if ( i > this->S/2 && i <= this->S ) {
                ef.leg[0].x = this->xo + 0.6*(this->S-i)*sin(-alpha);      
                ef.leg[0].y = this->yo + 0.6*(this->S-i)*cos(-alpha);      
                ef.leg[0].z = this->zo;
            }
            // Third Stage: S/3 < i <= S
            if ( i > this->S/3 && i <= this->S ) {
                ef.leg[2].x = this->xo + 0.6*(this->S - i)*sin(-alpha);      
                ef.leg[2].y = 0 + 0.6*(this->S - i)*cos(-alpha);       
                ef.leg[2].z = this->zo;          
            }
            // Third Sgage: 5*S/6 < i <= S
            if ( i > 5*this->S/6 && i <= this->S ) {
                ef.leg[3].x = this->xo + 0.6*(this->S - i)*sin(alpha);       
                ef.leg[3].y = 0 + 0.6*(this->S - i)*cos(alpha);        
                ef.leg[3].z = this->zo;
            }
            // Third Stage: 2*S/3 < i <= S
            if ( i > 2*this->S/3 && i <= this->S ) {
                ef.leg[5].x = this->xo + 0.6*(this->S - i)*sin(alpha);       
                ef.leg[5].y = -this->yo + 0.6*(this->S - i)*cos(alpha);      
                ef.leg[5].z = this->zo;
            }
            break;
        default :
            cout << "ERROR: Undefined Walk " << walk << "." << endl;
            return 0;
            break;
    }
    
    // TODO : Avoid /100 by /100 xo, yo, zo
    ef.leg[0].x /= 100;
    ef.leg[0].y /= 100;
    ef.leg[0].z /= 100;
    ef.leg[1].x /= 100;
    ef.leg[1].y /= 100;
    ef.leg[1].z /= 100;
    ef.leg[2].x /= 100;
    ef.leg[2].y /= 100;
    ef.leg[2].z /= 100;
    ef.leg[3].x /= 100;
    ef.leg[3].y /= 100;
    ef.leg[3].z /= 100;
    ef.leg[4].x /= 100;
    ef.leg[4].y /= 100;
    ef.leg[4].z /= 100;
    ef.leg[5].x /= 100;
    ef.leg[5].y /= 100;
    ef.leg[5].z /= 100;
    
    this->ef_position = ef;
    
    #if defined(__LOCOMOTION_DEBUG) || defined(__ALL_DEBUG)
    cout << 1 << "," << ef.leg[0].x << "," << ef.leg[0].y << "," << ef.leg[0].z << endl;
    cout << 2 << "," << ef.leg[1].x << "," << ef.leg[1].y << "," << ef.leg[1].z << endl;
    cout << 3 << "," << ef.leg[2].x << "," << ef.leg[2].y << "," << ef.leg[2].z << endl;
    cout << 4 << "," << ef.leg[3].x << "," << ef.leg[3].y << "," << ef.leg[3].z << endl;
    cout << 5 << "," << ef.leg[4].x << "," << ef.leg[4].y << "," << ef.leg[4].z << endl;
    cout << 6 << "," << ef.leg[5].x << "," << ef.leg[5].y << "," << ef.leg[5].z << endl;
    #endif
    
    return 1;
}