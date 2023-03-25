#include "core_search.h"

Core::Core(const QString& path, const QVector<QString>& form)
    :format(form)
{
    auto str = path.toStdU16String();
    all_duplicate.reserve(2000);
    std::filesystem::path sendbox(str);
    auto in_direct = all_internal_directories(sendbox);
    //std::thread th(&Core::serach_all_files, this, std::ref(sendbox));//--------ЭТАЛОН
    std::vector<std::thread> arr_th;

    auto start_ch = std::chrono::high_resolution_clock::now();

    for(const auto& dir : in_direct){
        std::thread th(&Core::serach_all_files, this, std::ref(dir));
        arr_th.emplace_back(std::move(th));
    }
    qDebug() << arr_th.size() << "- Threads";
    for(auto& th : arr_th){
        th.join();
    }

    auto end_ch = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_ch - start_ch);
    qDebug() << duration.count() << " - ms form \"all_duplicate\" multi thread";
    start_ch = std::chrono::high_resolution_clock::now();

    std::vector<file_param> tmp;
    tmp.reserve(all_files_sorted.size());
    std::sort(std::execution::par, all_duplicate.begin(), all_duplicate.end());
    for(const auto& param : all_files_sorted){
        if(std::binary_search(all_duplicate.begin(), all_duplicate.end(), param)){
            tmp.push_back(param);
        }
    }
    //all_duplicate.insert(all_duplicate.cend(), tmp.begin(), tmp.end());

    end_ch = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_ch - start_ch);
    qDebug() << duration.count() << " - ms add in \"all_duplicate\" last element";
    start_ch = std::chrono::high_resolution_clock::now();

    auto m_vect(all_duplicate);
    all_duplicate.reserve(tmp.size() + all_duplicate.size());
    all_duplicate.clear();
    std::merge(m_vect.begin(), m_vect.end(), tmp.begin(), tmp.end(), std::back_inserter(all_duplicate));
    //std::sort(std::execution::par, all_duplicate.begin(), all_duplicate.end());//Mearge!!!!

    end_ch = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_ch - start_ch);
    qDebug() << duration.count() << " - ms merge";
    start_ch = std::chrono::high_resolution_clock::now();

    auto it_a = all_duplicate.begin();
    int dupl_counter = 1;
    for(auto it = all_duplicate.begin(); it != all_duplicate.end(); ++it){
        if(*it == *it_a){
            it->counter = dupl_counter;
        }
        else{
            it_a = it;
            ++dupl_counter;
            it->counter = dupl_counter;
        }
    }

    end_ch = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_ch - start_ch);
    qDebug() << duration.count() << " - ms final form and numering";

}

QVector<std::filesystem::path> Core::all_internal_directories(const std::filesystem::path& sandbox){
    namespace fs = std::filesystem;
    fs::directory_iterator next(sandbox, fs::directory_options::skip_permission_denied), end;

    QVector<fs::path> sandbox_direct;
    sandbox_direct.reserve(20);
    while(next != end){
        if(next->is_directory()){
            sandbox_direct.push_back(next->path());
        }
        else{
            try{
                if(next->is_regular_file()){
                    size_t size_prev = all_files_sorted.size();
                    auto tmp_param = generated_param(next);
                    all_files_sorted.insert(tmp_param);
                    if(size_prev == all_files_sorted.size()){
                        all_duplicate.push_back(tmp_param);
                    }
                }
            }
            catch(std::exception& e){
                qDebug() << e.what() << " <---> " <<"Error in Core::all_internal_directories";
            }
        }
        ++next;
    }
    return sandbox_direct;
}

void Core::serach_all_files(const std::filesystem::path& sandbox){

    auto start_ch = std::chrono::high_resolution_clock::now();//

    namespace fs = std::filesystem;
    //using namespace std::chrono;
    fs::recursive_directory_iterator next, end;
    try{
        fs::recursive_directory_iterator tmp_next(sandbox, fs::directory_options::skip_permission_denied), tmp_end;
        next = tmp_next;
        end = tmp_end;
    }
    catch(std::exception& e){
        qDebug() << e.what() << " <---> " <<"Error in Core::serach_all_files";
    }
    //qDebug() << std::distance(next, end);
    while(next != end){
        try{
            if(next->is_directory() || next->file_size() == 0){
                ++next;
                continue;
            }
            bool flag = false;
            for(const auto& format_elem : format){
                flag = (format_elem.toStdString() == next->path().string().substr(next->path().string().size() - 4));
                if(flag)
                    break;
            }

            if (flag) {
                auto tmp_param = generated_param(next);
                m.lock();
                size_t size_prev = all_files_sorted.size();
                all_files_sorted.insert(tmp_param);
                if(size_prev == all_files_sorted.size()){
                    all_duplicate.push_back(tmp_param);
                }
                m.unlock();
            }
            ++next;
        }
        catch(std::exception& e){
            ++next;
        }
    }

    auto end_ch = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_ch - start_ch);
    qDebug() << duration.count() << "- in thread " << QString::fromStdString(sandbox.string());
}

size_t Core::GetSize(){
    return all_duplicate.size();
}

std::vector<file_param> Core::get_duplicate(){
    return all_duplicate;
}
