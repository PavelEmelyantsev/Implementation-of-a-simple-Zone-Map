#include "zonemaps.h"

template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, uint _num_elements_per_zone) {
	// Set "num_elements_per_zone" member variable value.
    this->num_elements_per_zone = _num_elements_per_zone;
    
    // Set "elements" member variable value.
    for (int i = 0; i < _elements.size(); i ++) {
    	elements.push_back(_elements[i]);
	}
}

template<typename T>
void zonemap<T>::sort_elements() {
	// Sort elements
	sort(this->elements.begin(), this->elements.end());
}

template<typename T>
void zonemap<T>::build() {
	//this->sort_elements();
	// Set "num_zones" variable value from elements and num_elements_per_zone
	this->num_zones = this->elements.size() / this->num_elements_per_zone;
	
	// Make zones
	for (int i = 0; i < this->num_zones; i ++) {
		
		struct zone<T> z;
		
		// Set elements for each zone.
		for (int j = 0; j < this->num_elements_per_zone; j ++) {
			z.elements.push_back(this->elements[this->num_elements_per_zone * i + j]);
		}
		
		// Set min and max value of the zone. (min value is first value of the zone and max value is last value of the zone because elements are sorted.)
		z.min = this->elements[this->num_elements_per_zone * i];
		z.max = this->elements[this->num_elements_per_zone * (i + 1) - 1];
		
		// Set the size of the zone
		z.size = num_elements_per_zone;
		
		// Add a new zone
		this->zones.push_back(z);
	}
	
	// if the last zone that is smaller than num_elements_per_zone exists 
	if (this->elements.size() % this->num_elements_per_zone != 0) {
		struct zone<T> z;
		for (int j = this->num_zones * this->num_elements_per_zone; j < this->elements.size(); j ++) {
			z.elements.push_back(this->elements[j]);
		}
		z.min = this->elements[this->num_zones * this->num_elements_per_zone];
		z.max = this->elements[this->elements.size() - 1];
		
		// Add the last zone.
		this->zones.push_back(z);
		
		// Increase the number of zones.
		this->num_zones ++;
	}
}

// Find the value from zones by Binary Search
template<typename T>
bool zonemap<T>::query(T key) {
	int st = 0, end = this->num_zones - 1, mid, pos = -1;
	
	// Search the zone from zones by Binary Search
	while (st <= end) {
		mid = (st + end) / 2;
		
		// If the value exist into the zone exception first and last value
		if (key > this->zones[mid].min && key < this->zones[mid].max) {
			
			pos = mid;
			st = 0, end = this->zones[pos].size - 1;
			
			// Search the value from the zone by Binary Search
			while ( st <= end ) {
				mid = (st + end) / 2;
				T val = this->zones[pos].elements[mid];
				
				// If the value exists
				if (key == val) {
					return true;
				}
				if (key < val) {
					end = mid - 1;
				} else {
					st = mid + 1;
				}
			}
			
			// The value doesn't exist
			return false;
		}
		
		// If the value equals with min or max value of the zone
		if (key == this->zones[mid].min || key == this->zones[mid].max) {
			return true;
		}
		if (key < this->zones[mid].min) {
			end = mid - 1;
		} else {
			st = mid + 1;
		}
		
	}
    return false;
}

// Find the first postion of the value that is greater tnan or equal to the key value from zones by Binary Search
template<typename T>
int zonemap<T>::find_pos(T key) {
	int res = -1;
	int st = 0, end = this->num_zones - 1, mid, pos = -1;
	
	// Find the zone from zones by Binary Search
	while (st <= end) {
		mid = (st + end) / 2;
		
		// If the value exist into the zone exception first and last value
		if (key > this->zones[mid].min && key < this->zones[mid].max) {
			break;
		}
		
		// If the value equals with min or max value of the zone
		if (key == this->zones[mid].min || key == this->zones[mid].max) {
			// Find first zone that have the same value
			while (mid > 0 && this->zones[mid - 1].max == key) {
				mid --;
			}
			break;
		}
		if (key < this->zones[mid].min) {
			end = mid - 1;
		} else {
			st = ++ mid;
		}
	}
	
	// Set the postion of the zone.
	pos = mid;
	res = pos * this->num_elements_per_zone;
	
	// If the zone can not be found
	if (mid == this->num_zones) {
		return res;
	}

	mid = 0;
    
    if (res != -1) {
    	st = 0, end = this->zones[pos].size - 1;
    	
    	// Find the position from the zone by Binary Search
    	while (st <= end) {
    		mid = (st + end) / 2;
    		if (key <= this->zones[pos].elements[mid]) {
    			end = mid - 1;
			} else {
				st = ++ mid;
				
			}
		}
	}
	
	res += mid;
	
	return res;
}

template<typename T>
std::vector<T> zonemap<T>::query(T low, T high) {
	std::vector<T> res;
	
	// Get the postion of the low limit.
	int st = this->find_pos(low);
	
	// Get the next postion of the high limit.
	int end = this->find_pos(high + 1);
	
	// Save the elements from the low limit to the high limit
	for (int i = st; i < end; i ++) {
		res.push_back(this->elements[i]);
	}
	
	return res;
}


template<typename T>
T zonemap<T>::get_element(int pos) {
	return this->elements[pos];
}
//
//template<typename T>
//void zonemap<T>::print() {
//	
//	for (int i = 0; i < this->elements.size(); i ++) {
//		printf("%d\n", this->elements[i]);
//	}
//}
//
//template<typename T>
//void zonemap<T>::print_zones() {
//	for (int i = 0; i < this->num_zones; i ++) {
//		printf("-----%d------------\n", i + 1);
//		for (int j = 0; j < this->zones[i].size; j ++) {
//			printf("%d\n", this->zones[i].elements[j]);
//		}
//	}
//}

