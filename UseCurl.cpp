/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UseCurl.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 21:59:44 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 22:42:37 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UseCurl.hpp"

UseCurl::UseCurl(void)
{
	std::cout << "Constructeur de UseCurl utilisé." << std::endl;
}

UseCurl::UseCurl(const UseCurl &src)
{
	std::cout << "Constructeur de recopie UseCurl utilisé." << std::endl;
	*this = src;
}

const UseCurl &UseCurl::operator=(const UseCurl &src)
{
	std::cout << "Operatot= de UseCurl utilisé." << std::endl;
    if (this == &src)
        return *this;
    return *this;
}

UseCurl::~UseCurl(void)
{
	std::cout << "Destructeur de UseCurl utilisé." << std::endl;
}


std::string UseCurl::getHtml(const std::string& url)
{
	std::string		buffer;
	CURL *curl = curl_easy_init();
	
	if (curl)
	{
		CURLcode result;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
		result = curl_easy_perform(curl);
		if (result != CURLE_OK)
			std::cerr << "Error: " << curl_easy_strerror(result) << std::endl;
    	curl_easy_cleanup(curl);
	}
	
	return buffer;
}

size_t	UseCurl::_writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t			realsize;
	std::string		*buffer;
	
	realsize = size * nmemb;
	buffer = static_cast<std::string *>(userp);
	buffer->append(static_cast<char *>(contents), realsize);
	return realsize;
}
