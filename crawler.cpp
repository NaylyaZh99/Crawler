#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>

static size_t write_data(char *data, size_t size_data, size_t size_buf, std::string* buf) {
    if (buf) {
        buf->append(data, size_data * size_buf);
        return size_data * size_buf;
    }
    else {
		return 0;
	}
}

void fillLevel(std::vector< std::vector<std::string> > &urlByDepth, int depth, int curLevel, std::string &buf) {
	int end, len;
	for (int i = 0; i < (int)buf.size() - 4; i++) {
		if (buf.substr(i, 4) == "http") {
			end = i + 5;
			while (buf[end] != '"' && buf[end] != ' ' && buf[end] != '\n') {
				end++;
			}
			len = end - i;
			urlByDepth[curLevel + 1].push_back(buf.substr(i, len));
			std::cout << buf.substr(i, len) << std::endl;
		}
	}
}

int main(void) {
	std::string buf;
	int depth;
	std::string root;
	std::cout << "enter main URL and depth:" << std::endl;
	std::cin >> depth >> root; 
	std::vector< std::vector<std::string> > urlByDepth;
	urlByDepth.resize(depth + 1);
	urlByDepth[0].push_back(root);
	for (int i = 0; i < depth; i++) {
		for (int j = 0; j < (int)urlByDepth[i].size(); j++) {
			CURL*  handle = curl_easy_init();
			if (handle) {
				curl_easy_setopt(handle, CURLOPT_URL, urlByDepth[i][j].c_str());
				curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
				curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buf);
				CURLcode res = curl_easy_perform(handle);
				fillLevel(urlByDepth, depth, i, buf);
				curl_easy_cleanup(handle);
			}
		}
	}
    return 0;
}
