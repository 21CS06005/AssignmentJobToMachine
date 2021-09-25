#include <vector>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

enum Status {
    uncrossed,
    crossed,
    intersected,
    zero_matched
};

struct Element {
    int value;
    Status status;
};

struct Assignment {
    int x;
    int y;
};

// Generating random matrix of given size
vector<vector<Element>> generate_random_matrix(int size) {
    vector<vector<Element>> arr;
    for (int i = 0; i < size; ++i) {
        vector<Element> a;
        for (int j = 0; j < size; ++j) {
            Element e = { 1 + int(rand()) % 20, uncrossed};
            a.push_back(e);
        }
        arr.push_back(a);
    }

    return arr;
}

// Getting minimum value from the array of elements
int get_min_array(vector<Element> vect) {
    int min_value = INT_MAX;
    for (auto i = 0; i < 5; ++i) {
        if(vect[i].value < min_value) {
            min_value = vect[i].value;
        }
    }
    return min_value;
}

// Subtraction minimum value from the corresponding row or column
vector<vector<Element>> sub_min(vector<vector<Element>> arr, int size, bool inverted) {
    // If inverted is yes, it's col-wise min subtraction. Otherwise, it's a row-wise subtraction.
    int min = 0;
    vector<Element> vect;
    for (int i = 0; i < arr.size(); i++) {
        if(inverted) {
            vector<Element> temp;
            for (int j = 0; j < size; ++j) {
                temp.push_back(arr[j][i]);
            }
            vect = temp;
        }
        else {
            vect = arr[i];
        }

        min = get_min_array(vect);
        for (int j = 0; j < vect.size(); ++j) {
            vect[j].value = vect[j].value - min;
        }
        if(inverted) {
            for (int j = 0; j < size; ++j) {
                arr[j][i] = vect[j];
            }
        }
        else
            arr[i] = vect;
    }
    return arr;
}

// Getting the index of zero element in the given array.
int get_index_zero(vector<Element> vect) {
    //If there is only 1 zero, it returns the index of the zero. Otherwise, it returns -1.
    int index = -1, count = 0;

    bool flag = true;
    for (int j = 0; j < vect.size(); ++j) {
        if(vect[j].status == zero_matched)
            flag = false;
    }
    if(!flag)
        return -1;

    for (int i = 0; i < vect.size(); ++i) {
        if(vect[i].value == 0 && vect[i].status == uncrossed) {
            count++;
            index = i;
        }
    }
    if(count == 1)
        return index;
    else
        return -1;
}

// Crossing the columns based on counts of zero in rows
vector<vector<Element>> row_wise_crossing(vector<vector<Element>> arr, int size, bool* flag) {
    bool changes_done = false;
    //For each row, if the count of uncrossed zero is 1, cross out the corresponding columns.
    for (int i = 0; i < size; ++i) {
        vector<Element> vect;
        for (int j = 0; j < size; ++j) {
            vect.push_back(arr[i][j]);
        }
        int place = get_index_zero(vect);
        if(place != -1) {
            *flag = true;
            for (int j = 0; j < size; ++j) {
                arr[j][place].status = crossed;
            }
            arr[i][place].status = zero_matched;
        }
    }
    return arr;
}

// Crossing the rows based on counts of zero in columns
vector<vector<Element>> col_wise_crossing(vector<vector<Element>> arr, int size, bool* flag) {
    //For each column, if the count of uncrossed zero is 1, cross out the corresponding rows.
    for (int i = 0; i < size; ++i) {
        vector<Element> vect;
        for (int j = 0; j < size; ++j) {
            vect.push_back(arr[j][i]);
        }

        int place = get_index_zero(vect);
        if(place != -1) {
            *flag = true;
            for (int j = 0; j < size; ++j) {
                if(arr[place][j].status == crossed)
                    arr[place][j].status = intersected;
                else
                    arr[place][j].status = crossed;
            }
            arr[place][i].status = zero_matched;
        }
    }
    return arr;
}

vector<vector<Element>> handle_deadlock(vector<vector<Element>> arr, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if(arr[i][j].status == uncrossed && arr[i][j].value == 0) {
                for (int k = 0; k < size; ++k) {
                    if(arr[i][k].status == crossed)
                        arr[i][k].status = intersected;
                    else
                        arr[i][k].status = crossed;
                }
                arr[i][j].status = zero_matched;
                return arr;
            }
        }
    }
    return arr;
}

// Resetting the status of the elements to uncrossed.
vector<vector<Element>> reset_algorithm(vector<vector<Element>> arr, int size) {
    int min_value = INT_MAX;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if(arr[i][j].status == uncrossed) {
                if(arr[i][j].value < min_value)
                    min_value = arr[i][j].value;
            }
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if(arr[i][j].status == uncrossed) {
                arr[i][j].value -= min_value;
            }
            else if(arr[i][j].status == intersected) {
                arr[i][j].value += min_value;
            }
            arr[i][j].status = uncrossed;
        }
    }
    return arr;
}

// Checking if all the jobs are matched with machines
bool check_algorithm_complete(vector<vector<Element>> arr, int size) {
    int count = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if(arr[i][j].status == zero_matched)
                count++;
        }
    }
    if(count == size)
        return true;
    else
        return false;
}

// Checking if all the zeros are crossed.
bool check_uncrossed_zero(vector<vector<Element>> arr, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if(arr[i][j].value == 0 && arr[i][j].status == uncrossed) {
                return true;
            }
        }
    }
    return false;
}

// Assignment of jobs to machines
vector<Assignment> assign_job_to_machine(vector<vector<Element>> arr, int size) {
    vector<Assignment> assigned;
    bool algo_finished = false;
    while(!algo_finished) {
        bool zeros_updated = false;
        bool uncrossed_zero_exists = true;
        //    Subtracting min value row-wise
        arr = sub_min(arr, size, false);
        //    Subtracting min value col-size
        arr = sub_min(arr, size, true);
        int count = 0;
        while(uncrossed_zero_exists) {
            count++;
            zeros_updated = false;
            //  Row-wise crossing
            arr = row_wise_crossing(arr, size, &zeros_updated);
            //  Column-wise crossing
            arr = col_wise_crossing(arr, size, &zeros_updated);

            while(!zeros_updated && count > 1) {
                arr = handle_deadlock(arr, size);
                zeros_updated = true;
            }
            uncrossed_zero_exists = check_uncrossed_zero(arr, size);
        }

        algo_finished = check_algorithm_complete(arr, size);
        if(!algo_finished) {
            arr = reset_algorithm(arr, size);
        }
    }

    for (int i = 0; i < arr.size(); ++i) {
        for (int j = 0; j < arr.size(); ++j) {
            if(arr[i][j].status == zero_matched) {
                Assignment assignment = {i, j};
                assigned.push_back(assignment);
            }
        }
    }
    return assigned;
}

int main() {
    srand(time(0));
    int size = 0;
    vector<vector<Element>> matrix;

    cout<<"Enter the number of jobs"<<endl;
    cin>>size;

    matrix = generate_random_matrix(size);
    cout<<endl<<"Jobs vs Machines are:"<<endl;
    for (vector<Element> vect1D : matrix) {
        for (Element e : vect1D) {
            cout<<e.value<<"\t";
        }
        cout<<endl;
    }

    vector<Assignment> result = assign_job_to_machine(matrix, size);
    int total_cost = 0;
    cout<<endl<<endl<<"Assignment:"<<endl;
    for (int i = 0; i < result.size(); ++i) {
        cout<<"Job-"<<result[i].x + 1<<" -> Machine-"<<result[i].y + 1<<endl;
        total_cost += matrix[result[i].x][result[i].y].value;
    }
    cout<<endl<<"Minimum cost for matching: "<<total_cost<<endl;
    return 0;
}
