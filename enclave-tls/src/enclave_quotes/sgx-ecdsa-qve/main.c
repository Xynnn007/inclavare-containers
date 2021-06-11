/* Copyright (c) 2020-2021 Alibaba Cloud and Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <enclave-tls/enclave_quote.h>
#include <enclave-tls/log.h>

extern enclave_quote_err_t enclave_quote_register(enclave_quote_opts_t *opts);
extern enclave_quote_err_t sgx_ecdsa_pre_init(void);
extern enclave_quote_err_t sgx_ecdsa_qve_init(enclave_quote_ctx_t *ctx,
					      enclave_tls_cert_algo_t algo);
extern enclave_quote_err_t sgx_ecdsa_collect_evidence(enclave_quote_ctx_t *ctx,
						      attestation_evidence_t *evidence,
						      enclave_tls_cert_algo_t algo, uint8_t *hash);
extern enclave_quote_err_t sgx_ecdsa_verify_evidence(enclave_quote_ctx_t *ctx,
						     attestation_evidence_t *evidence,
						     uint8_t *hash, uint32_t hash_len);
extern enclave_quote_err_t sgx_ecdsa_cleanup(enclave_quote_ctx_t *ctx);

static enclave_quote_opts_t sgx_ecdsa_qve_opts = {
	.api_version = ENCLAVE_QUOTE_API_VERSION_DEFAULT,
	.flags = ENCLAVE_QUOTE_OPTS_FLAGS_SGX_ENCLAVE,
	.name = "sgx_ecdsa_qve",
	.type = "sgx_ecdsa",
	.priority = 53,
	.pre_init = sgx_ecdsa_pre_init,
	.init = sgx_ecdsa_qve_init,
	.collect_evidence = sgx_ecdsa_collect_evidence,
	.verify_evidence = sgx_ecdsa_verify_evidence,
	.cleanup = sgx_ecdsa_cleanup,
};

#ifdef SGX
void libenclave_quote_sgx_ecdsa_qve_init(void)
#else
void __attribute__((constructor)) libenclave_quote_sgx_ecdsa_qve_init(void)
#endif
{
	ETLS_DEBUG("called\n");

	enclave_quote_err_t err = enclave_quote_register(&sgx_ecdsa_qve_opts);
	if (err != ENCLAVE_QUOTE_ERR_NONE)
		ETLS_DEBUG("failed to register the enclave quote 'sgx_ecdsa_qve' %#x\n", err);
}
